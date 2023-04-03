#include <iostream>
#include <rom/romulus.h>
#include <sparkle/sparkle.h>

int main(){
	std::cout << "Hello \n";

    char* ptr;
    romulus::test(reinterpret_cast<uint8_t*>(ptr));

    std::cout << ptr << '\n';
    std::cout << ptr << '\n';
    std::cout << sparkle::rotateShift(32, 5) << '\n';

	return 0;
}

