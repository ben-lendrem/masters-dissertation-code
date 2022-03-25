#include "../nclgl/Vector3.h"
#include <memory>
#include <iostream>
int main(void) {
	std::unique_ptr<bool[]> x = std::make_unique<bool[]>(5);
	for (int i = 0; i < 5; i++) {
		std::cout << x[i] << std::endl;
	}
	return 0;
}