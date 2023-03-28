#pragma once
#include <stdint.h>
#include <memory>

namespace romulus {
	uint32_t encrypt(uint32_t* in, uint32_t* key, uint32_t* out);
	uint32_t pad();
    uint32_t test(uint8_t* ptr);
	
};



