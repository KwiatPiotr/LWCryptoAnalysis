#include <iostream>
#include <rom/romulus.h>
#include <sparkle/sparkle.h>

int main(){
	std::cout << "Hello \n";
	std::cout << "Hello \n";
	std::cout << "Hello \n";

    uint32_t state[2*sparkle::MAX_BRANCHES] = {0};

    sparkle::printState(state, sparkle::MAX_BRANCHES);

    sparkle::encrypt(state, 4, sparkle::MAX_BRANCHES);

    std::cout << "======================================\n";

    sparkle::printState(state, sparkle::MAX_BRANCHES);

	return 0;
}

