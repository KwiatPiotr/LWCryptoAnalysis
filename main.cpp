#include <iostream>

#include "sparkle/sparkle.h"
#include "schwaemm/schwaemm.h"
#include "api.h"

#define MAX_MESSAGE_LENGTH  32
#define MAX_AD_LENGTH       32

int main(){
    // Count = 17
    // Key = 000102030405060708090A0B0C0D0E0F
    // Nonce = 000102030405060708090A0B0C0D0E0F
    // PT = 
    // AD = 000102030405060708090A0B0C0D0E0F
    // CT = 8B7AEE52D40C7E0EDF9CB56FFAE5D882

    schwaemm::UChar ct[MAX_MESSAGE_LENGTH + CRYPTO_ABYTES];
    schwaemm::UChar msg[MAX_MESSAGE_LENGTH];
    schwaemm::UChar ad[MAX_AD_LENGTH];
    schwaemm::UChar nonce[CRYPTO_NPUBBYTES];
    schwaemm::UChar key[CRYPTO_KEYBYTES];
    schwaemm::ULLInt clen;
    schwaemm::ULLInt mlen = MAX_MESSAGE_LENGTH;
    schwaemm::ULLInt adlen = MAX_AD_LENGTH;

    schwaemm::init_buffer(key, sizeof(key));
    schwaemm::init_buffer(nonce, sizeof(nonce));
    schwaemm::init_buffer(msg, sizeof(msg));
    schwaemm::init_buffer(ad, sizeof(ad));

    int func_ret = schwaemm::crypto_aead_encrypt(ct, &clen, msg, mlen, ad, adlen, NULL, nonce, key);

    printf("==========================================\n");
    printf("%s\n", ct);
    printf("==========================================\n");

	return 0;
}

