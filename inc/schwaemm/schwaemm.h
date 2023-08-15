#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <cstring>
#include <sparkle/sparkle.h>
#include <stdalign.h>

namespace schwaemm {
    typedef unsigned char UChar;
    typedef unsigned long long int ULLInt;

    constexpr uint32_t ALIGN_OF_UI32 =  alignof(uint32_t);


    constexpr uint32_t SCHWAEMM_KEY_LEN = 16;
    constexpr uint32_t SCHWAEMM_NONCE_LEN = 16;
    constexpr uint32_t SCHWAEMM_TAG_LEN = 16;
    constexpr uint32_t STATE_BYTES = 32;
    constexpr uint32_t RATE_BYTES = 16;
    constexpr uint32_t STEPS_SLIM = 7;
    constexpr uint32_t STEPS_BIG = 10;

    constexpr uint32_t SCHWAEMM_KEY_WORDS = (SCHWAEMM_KEY_LEN/4);
    constexpr uint32_t SCHWAEMM_NONCE_WORDS = (SCHWAEMM_NONCE_LEN/4);
    constexpr uint32_t SCHWAEMM_TAG_WORDS = (SCHWAEMM_TAG_LEN/4);

    constexpr uint32_t STATE_BRANS = (STATE_BYTES/8);
    constexpr uint32_t STATE_WORDS = (STATE_BYTES/4);
    constexpr uint32_t RATE_BRANS = (RATE_BYTES/8);
    constexpr uint32_t RATE_WORDS = (RATE_BYTES/4);
    constexpr uint32_t CAP_BYTES = (STATE_BYTES-RATE_BYTES);
    constexpr uint32_t CAP_BRANS = (CAP_BYTES/8);
    constexpr uint32_t CAP_WORDS = (CAP_BYTES/4);

    constexpr uint32_t CONST_A0 = (((uint32_t) (0 ^ (1 << CAP_BRANS))) << 24);
    constexpr uint32_t CONST_A1 = (((uint32_t) (1 ^ (1 << CAP_BRANS))) << 24);
    constexpr uint32_t CONST_M2 = (((uint32_t) (2 ^ (1 << CAP_BRANS))) << 24);
    constexpr uint32_t CONST_M3 = (((uint32_t) (3 ^ (1 << CAP_BRANS))) << 24);


    void init(uint32_t* state, const uint8_t* key, const uint8_t* nounce);

    void processAData(const uint8_t* in, size_t len, uint32_t* state);

    void processPlain(const uint8_t* in, uint8_t* out, size_t len, uint32_t* state);

    void processCipher(uint8_t* in, uint8_t* out, size_t len, uint32_t* state);

    void finilize(uint32_t* state, const uint8_t* key);

    void generateTag(uint32_t* state, uint8_t* tag);

    void verifyTag(uint32_t* state, uint8_t* tag);

    int crypto_aead_encrypt(UChar *c, ULLInt *clen, const UChar *m, ULLInt mlen,    \
        const UChar *ad, ULLInt adlen, const UChar *nsec, const UChar *npub,        \
        const UChar *k
    );

    void rho_W_opt_aut(uint32_t *state, const uint8_t *in, int castable);

    void rho_W_opt_enc(uint32_t *state, uint8_t *out, const uint8_t *in, int castable);

    void init_buffer(UChar *buffer, ULLInt numbytes);
};
