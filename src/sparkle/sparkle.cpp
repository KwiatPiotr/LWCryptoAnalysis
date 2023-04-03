#include "sparkle/sparkle.h"


uint32_t sparkle::rotateShift(uint32_t x, uint32_t shift) {
    return (x >> shift) | (x << (32 - shift));
}

uint32_t sparkle::permutation(uint32_t x) {
    return rotateShift(x ^ (x << 16), 16);
}

uint32_t sparkle::encrypt(uint8_t* state, uint32_t* state, uint32_t* params){ 
    uint8_t i, j;
    uint32_t roundCounter, tempX, tempY, firstX, firstY;

    for (i = 0 ; i < steps ; i++) {
        state[1] = ROUND_CONSTANTS[i%MAX_BRANCHES];
        state[3] = i;

        // Alzette ARX-box
        for (j = 0 ; i < 2 * branches ; j++) {
            roundCounter = ROUND_CONSTANTS[j>>1];
            state[j] += rotateShift(state[j+1], 31);
            state[j+1] ^= rotateShift(state[j], 24);
            state[j] ^= roundCounter;
            state[j] += rotateShift(state[j+1], 17);
            state[j+1] ^= rotateShift(state[j], 17);
            state[j] ^= roundCounter;
            state[j] += state[j+1];
            state[j+1] ^= rotateShift(state[j], 31);
            state[j] ^= roundCounter;
            state[j] += rotateShift(state[j+1], 24);
            state[j+1] ^= rotateShift(state[j], 16);
            state[j] ^= roundCounter;
        }

        // Linear Function
        tempX = state[0];
        tempY = state[1];
        for(j = 2; j < brans; j += 2) {
            tempX ^= state[j];
            tempY ^= state[j+1];
        }
        tempX = permutation(tempX);
        tempY = permutation(tempY);
    }

    return 0;
}

uint32_t sparkle::decrypt(uint8_t* state, uint32_t* state, uint32_t* params){
    return 0;
}

inline uint32_t sparkle::substitution(){
    return 0;
}
