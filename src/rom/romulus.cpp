#include "rom/romulus.h"

namespace romulus {
    uint32_t pad(){
        return 0xff;
    }

	uint32_t encrypt(uint32_t* in, uint32_t* key, uint32_t* out){

    }

    uint32_t test(uint8_t* ptr) {
        ptr = static_cast<uint8_t*>(std::calloc(6, 1));
        ptr[0] = 'h';
        ptr[1] = 'e';
        ptr[2] = 'l';
        ptr[3] = 'l';
        ptr[4] = 'o';


        return 0;
    }
}

