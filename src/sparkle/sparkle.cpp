#include "sparkle/sparkle.h"


uint32_t sparkle::rotateShift(uint32_t x, uint32_t shift) {
    return (x >> shift) | (x << (32 - shift));
}

uint32_t sparkle::permutation(uint32_t x) {
    return rotateShift(x ^ (x << 16), 16);
}

