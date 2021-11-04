#include "regular.hpp"

void Regular::process()
{
	tree.process();
	tree.setNFL();
	tree.print();
	graph.process(tree);
	std::cout << "Before minimization:" << std::endl << std::endl;
	graph.print();
	graph.minimize();
	std::cout << "After minimization:" << std::endl << std::endl;
	graph.print();
}