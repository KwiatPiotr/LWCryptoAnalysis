#include "schwaemm/schwaemm.h"
#include "stdio.h"

namespace schwaemm {
    void init(uint32_t* state, const uint8_t* key, const uint8_t* nounce) {
        std::memcpy(state, nounce, 16);
        std::memcpy((state + RATE_WORDS), key, SCHWAEMM_KEY_LEN);
        sparkle::encrypt(state, STATE_BRANS, STEPS_BIG);
    }

    void processAData(const uint8_t* in, size_t len, uint32_t* state) {
        int aligned = ((uintptr_t) in) % (ALIGN_OF_UI32) == 0;
        int castable = (aligned) ? 0 : RATE_BYTES;

        while (len > RATE_BYTES) {
            rho_W_opt_aut(state, in, castable);
            sparkle::encrypt(state, STATE_BRANS, STEPS_SLIM);
            len -= RATE_BYTES;
            in += RATE_BYTES;
        }

        if (len == RATE_BYTES) {
            state[STATE_WORDS-1] ^= CONST_A1;
        }
        else {
            state[STATE_WORDS-1] ^= CONST_A0;
            castable = (int) len;
        }

        rho_W_opt_aut(state, in, castable);
        sparkle::encrypt(state, STATE_BRANS, STEPS_BIG);
    }

    void processPlain(const uint8_t* in, uint8_t* out, size_t len, uint32_t* state) {
        int aligned = (((uintptr_t) in) | ((uintptr_t) out)) % (ALIGN_OF_UI32) == 0;
        int castable = (aligned) ? 0 : RATE_BYTES;

        while (len > RATE_BYTES) {
            rho_W_opt_enc(state, out, in, castable);
            sparkle::encrypt(state, STATE_BRANS, STEPS_SLIM);
            len -= RATE_BYTES;
            out += RATE_BYTES;
            in += RATE_BYTES;
        }

        if (len == RATE_BYTES) {
            state[STATE_WORDS-1] ^= CONST_M3;
        }
        else {
            state[STATE_WORDS-1] ^= CONST_M2;
            castable = (int) len;
        }

        rho_W_opt_enc(state, out, in, castable);
        sparkle::encrypt(state, STATE_BRANS, STEPS_BIG);
    }

    void processCipher(uint8_t* in, uint8_t* out, size_t len, uint32_t* state) {

    }

    void finilize(uint32_t* state, const uint8_t* key) {
        uint32_t buffer[SCHWAEMM_KEY_WORDS];
        uint32_t *key32;
        int i, castable = ((uintptr_t) key) % (ALIGN_OF_UI32) == 0;

        if (castable) {
            key32 = (uint32_t *) (void *) key;
        }
        else {
            memcpy(buffer, key, SCHWAEMM_KEY_LEN);
            key32 = (uint32_t *) buffer;
        }

        for (i = 0; i < SCHWAEMM_KEY_WORDS; i++) {
            state[RATE_WORDS+i] ^= key32[i];
        }
    }

    void generateTag(uint32_t* state, uint8_t* tag) {

    }

    void verifyTag(uint32_t* state, uint8_t* tag) {

    }

    int crypto_aead_encrypt(UChar *c, ULLInt *clen, const UChar *m, ULLInt mlen,    \
        const UChar *ad, ULLInt adlen, const UChar *nsec, const UChar *npub,        \
        const UChar *k) {
        uint32_t state[STATE_WORDS];
        size_t msize = (size_t) mlen;
        size_t adsize = (size_t) adlen;
        
        (void) nsec;  // to get rid of a warning
        
        init(state, k, npub);
        if (adsize > 0) processAData(ad, adsize, state);
        if (msize > 0) processPlain(m, c, msize, state);
        finilize(state, k);
        generateTag(state, (c + msize));
        // printf("%u, %u, %u, %u, %u, %u, %u, %u\n", state[0], state[1], state[2], state[3], state[4], state[5], state[6], state[7]);
        // printf("%02X:%02X:%02X:%02X\n", c[0], c[1], c[2], c[3]);
        *clen = msize;
        *clen += SCHWAEMM_TAG_LEN;
        
        return 0;
    }


    void rho_W_opt_aut(uint32_t *state, const uint8_t *in, int castable) {
        uint32_t buffer[RATE_WORDS+1];
        uint32_t rw1, rw2;  // left/right rate-word
        uint32_t dw1, dw2;  // left/right data-word
        uint32_t *in32;
        int i;

        if (castable == 0) {  // `in` can be directly processed
            in32 = (uint32_t *) (void *) in;  // to prevent cast-warning
        }
        else {  // bytes of array `in` are copied to (padded) buffer
            i = (int) (castable >> 2);  // start-index of the 0-words
            memset((buffer + i), 0, 4*(RATE_WORDS - i));
            ((uint8_t *) buffer)[castable] = 0x80;
            memcpy(buffer, in, castable);
            in32 = (uint32_t *) buffer;
        }

        for (i = 0; i < RATE_WORDS/2; i++) {
            rw1 = state[i];
            rw2 = state[RATE_WORDS/2+i];
            dw1 = in32[i];
            dw2 = in32[RATE_WORDS/2+i];
            dw2 ^= rw2;  // part of Rho1
            rw1 ^= dw2;  // Feistel-XOR
            rw2 ^= dw1;  // part of Rho1
            rw2 ^= state[RATE_WORDS+i];
            rw1 ^= state[(STATE_WORDS-RATE_WORDS/2)+i];
            state[i] = rw2;
            state[RATE_WORDS/2+i] = rw1; 
        }
    }

    void rho_W_opt_enc(uint32_t *state, uint8_t *out, const uint8_t *in, int castable) {
        uint32_t buffer[RATE_WORDS+1];
        uint32_t rw1, rw2;  // left/right rate-word
        uint32_t dw1, dw2;  // left/right ptxt-word
        uint32_t *in32, *out32;
        int i;

        if (castable == 0) {   // `in`, `out` can be directly processed
            in32 = (uint32_t *) (void *) in;    // to prevent cast-warning
            out32 = (uint32_t *) (void *) out;  // to prevent cast-warning
        }
        else {  // bytes of array `in` are copied to a (padded) buffer
            i = (int) (castable >> 2);  // start-index of the 0-words
            memset((buffer + i), 0, 4*(RATE_WORDS - i));
            ((uint8_t *) buffer)[castable] = 0x80;
            memcpy(buffer, in, castable);
            in32 = out32 = (uint32_t *) buffer;
        }

        for (i = 0; i < RATE_WORDS/2; i++) {
            rw1 = state[i];
            rw2 = state[RATE_WORDS/2+i];
            dw1 = in32[i];
            dw2 = in32[RATE_WORDS/2+i];
            dw2 ^= rw2;  // part of Rho2
            rw2 ^= dw1;  // part of Rho1
            dw1 ^= rw1;  // part of Rho2
            rw1 ^= dw2;  // part of Rho1 + Feistel-XOR
            out32[i] = dw1;
            out32[RATE_WORDS/2+i] = dw2;
            // Rate-Whitening + Feistel-Swap rw1 <-> rw2
            rw2 ^= state[RATE_WORDS+i];
            rw1 ^= state[(STATE_WORDS-RATE_WORDS/2)+i];
            state[i] = rw2;
            state[RATE_WORDS/2+i] = rw1;
        }

        if (castable) memcpy(out, buffer, castable);
    }

    void init_buffer(UChar *buffer, ULLInt numbytes) {
        ULLInt i;

        for (i = 0; i < numbytes; i++)
            buffer[i] = (UChar)i;
    }
}
