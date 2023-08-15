#pragma once
#include <iostream>
#include <array>
#include <stdint.h>

namespace sparkle {
	static constexpr uint32_t MAX_BRANCHES = 8;

	static constexpr std::array<uint32_t, MAX_BRANCHES> ROUND_CONSTANTS = {
		0xB7E15162, 0xBF715880, 0x38B4DA56, 0x324E7738,
		0xBB1185EB, 0x4F7C7B57, 0xCFBFA1C8, 0xC2B3293D
	};

	inline uint32_t rotateShift(uint32_t x, uint32_t shift);
	inline uint32_t permutation(uint32_t x);
	inline uint32_t substitution();
	uint32_t encrypt(uint32_t* state, uint32_t branches, uint32_t steps);
	uint32_t decrypt(uint8_t* state, uint32_t steps, uint32_t branches);

    void printState(const uint32_t* state, int branches);


};

