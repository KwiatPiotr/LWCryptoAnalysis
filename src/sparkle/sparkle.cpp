#include "sparkle/sparkle.h"


uint32_t sparkle::rotateShift(uint32_t x, uint32_t shift) {
    return (x >> shift) | (x << (32 - shift));
}

uint32_t sparkle::permutation(uint32_t x) {
    return rotateShift(x ^ (x << 16), 16);
}

uint32_t sparkle::encrypt(uint32_t* state, uint32_t steps, uint32_t branches) {
    uint8_t i, j;
    uint32_t roundCounter, tempX, tempY, firstX, firstY;

    for (i = 0 ; i < steps ; i++) {
        state[1] ^= ROUND_CONSTANTS[i%MAX_BRANCHES];
        state[3] ^= i;

        // Alzette ARX-box
        for (j = 0 ; j < 2 * branches ; j += 2) {
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
        firstX = state[0];
        firstY = state[1];
        for(j = 2 ; j < branches ; j += 2) {
            tempX ^= state[j];
            tempY ^= state[j+1];
        }

        tempX = permutation(tempX);
        tempY = permutation(tempY);

        //    The Diffusion Layer
        for (j = 2 ; j < branches ; j += 2) {
            state[j-2] = state[j+branches] ^ state[j] ^ tempY;
            state[j+branches] = state[j];
            state[j-1] = state[j+branches+1] ^ state[j+1] ^ tempX;
            state[j+branches+1] = state[j+1];
        }
        state[branches-2] = state[branches] ^ firstX ^ tempY;
        state[branches] = firstX;
        state[branches-1] = state[branches+1] ^ firstY ^ tempX;
        state[branches+1] = firstY;
    }

    return 0;
}

uint32_t sparkle::decrypt(uint8_t* state, uint32_t steps, uint32_t branches){
    return 0;
}

inline uint32_t sparkle::substitution(){
    return 0;
}

void sparkle::printState(const uint32_t* state, int branches) {
    uint8_t* stateBytes = (uint8_t*)state;
    int j = 0;
    for (int i = 0 ; i < branches ; i++ ) {
        j = 8*i;
        printf("(%02x%02x%02x%02x %02x%02x%02x%02x)",       \
        stateBytes[j],   stateBytes[j+1], stateBytes[j+2], stateBytes[j+3], \
        stateBytes[j+4], stateBytes[j+5], stateBytes[j+6], stateBytes[j+7]);
        if (i < branches-1) printf(" ");
    }
}