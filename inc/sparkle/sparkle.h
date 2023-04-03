#pragma once
#include <array>
#include <stdint.h>

namespace sparkle {
	static constexpr uint32_t MAX_BRANCHES = 16;

	static constexpr std::array<uint32_t, MAX_BRANCHES> ROUND_CONSTANTS = {
		0xB7E15162, 0xBF715880, 0x38B4DA56, 0x324E7738,
		0xBB1185EB, 0x4F7C7B57, 0xCFBFA1C8, 0xC2B3293D
	};
	uint32_t rotateShift(uint32_t x, uint32_t shift);
	uint32_t permutation(uint32_t x);
	uint32_t encrypt(uint32_t* state, uint32_t* params);
	uint32_t decrypt(uint32_t* state, uint32_t* params);



};

