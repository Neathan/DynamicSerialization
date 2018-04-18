#include <iostream>

#include "DynamicSerialization.h"

struct Test {
	int a;
	float b;
	int c;
};

int main() {
	Test* t = new Test();

	DS::createStruct(t, 8, 3.141f, 32);
	
	std::cout << t->a << std::endl;
	std::cout << t->b << std::endl;
	std::cout << t->c << std::endl;

	DS::serializeToFile(t, "temp.dat");

	Test* b = new Test();
	DS::deserializeFromFile(b, "temp.dat");

	std::cout << b->a << std::endl;
	std::cout << b->b << std::endl;
	std::cout << b->c << std::endl;
	return 0;
}
