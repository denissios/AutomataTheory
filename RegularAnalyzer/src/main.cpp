#include "regular.hpp"

int main()
{
	//Regular regular1("a.b.#|c(d|efg|s&{5,}z)qwr+(3:a|(a&{0,}c)(d|efg|s&{3,6}.z)qwr)\\3");
	Regular regular2("(a|bda|abc){0,}abb");
	try {
		regular2.process();
	}
	catch (const char* ex) {
		std::cerr << ex << std::endl;
	}

	return 0;
}