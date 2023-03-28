#include <iostream>
#include <rom/romulus.h>

int main(){
	std::cout << "Hello \n";

    char* ptr;
    romulus::test(reinterpret_cast<uint8_t*>(ptr));

    std::cout << ptr << '\n';

	return 0;
}

