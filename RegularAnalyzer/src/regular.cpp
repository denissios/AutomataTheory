#include "regular.hpp"

void Regular::compile()
{
	if (is_compile)
		return;

	is_compile = true;
	tree.process();
	tree.setNFL();
	//tree.print();
	graph.process(tree);
	//std::cout << "Before minimization:" << std::endl << std::endl;
	//graph.print();
	graph.minimize();
	//std::cout << "After minimization:" << std::endl << std::endl;
	//graph.print();
}

bool Regular::match(std::string& str)
{
	std::vector<std::string > vec;
	return match(str, vec);
}

bool Regular::match(std::string& str, std::vector<std::string>& capture)
{
	if (!is_compile) {
		this->compile();
	}

	return graph.match(str, capture);
}

void Regular::findAll(std::string& str, std::vector<std::string>& strings)
{
	std::vector<std::string > vec;
	findAll(str, strings, vec);
}

void Regular::findAll(std::string& str, std::vector<std::string>& strings, std::vector<std::string>& capture)
{
	if (!is_compile) {
		this->compile();
	}

	graph.findAll(str, strings, capture, 0);
}

std::string Regular::getRE()
{
	if (!is_compile) {
		std::cout << "Automata hasn't compiled yet" << std::endl;
		return "";
	}

	return graph.getRE();
}

void Regular::subtraction(Regular& regular1, Regular& regular2)
{
	if (!regular1.is_compile) {
		regular1.compile();
	}
	if (!regular2.is_compile) {
		regular2.compile();
	}

	graph.multiple_for_sub(regular1.graph, regular2.graph);
	//std::cout << std::endl << "===============SUBTRACTION===============" << std::endl << std::endl;
	//graph.print();

	is_compile = true;
}

void Regular::inversion(Regular& regular)
{
	if (!is_compile) {
		regular.compile();
	}

	tree.process(regular.getRE());
	tree.startInversion();

	tree.setNFL();
	//tree.print();
	graph.process(tree);
	//std::cout << "Before minimization:" << std::endl << std::endl;
	//graph.print();
	graph.minimize();
	//std::cout << "After minimization:" << std::endl << std::endl;
	//graph.print();

	is_compile = true;
}