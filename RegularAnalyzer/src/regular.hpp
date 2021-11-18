#ifndef __REGULAR_HPP_INCLUDED__
#define __REGULAR_HPP_INCLUDED__

#include "synTree.hpp"
#include "graph.hpp"

class Regular
{
public:
	Regular(std::string regular) : tree{ regular }, is_compile{ false } {}
	void compile();
	bool match(std::string& str, std::vector<std::string>& capture);
	bool match(std::string& str);
	void findAll(std::string& str, std::vector<std::string>& strings, std::vector<std::string>& capture);
	void findAll(std::string& str, std::vector<std::string>& strings);
	std::string getRE();
	void subtraction(Regular& regular1, Regular& regular2);
	void inversion(Regular& regular);
	[[nodiscard]] inline constexpr Graph& getGraph() { return graph; }

private:
	bool is_compile;
	SynTree tree;
	Graph graph;
};



#endif //__REGULAR_HPP_INCLUDED__