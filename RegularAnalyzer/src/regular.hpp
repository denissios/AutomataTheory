#ifndef __REGULAR_HPP_INCLUDED__
#define __REGULAR_HPP_INCLUDED__

#include "synTree.hpp"
#include "graph.hpp"

class Regular
{
public:
	Regular(std::string regular) : tree{ regular } {}
	void process();

private:

	SynTree tree;
	Graph graph;
};



#endif //__REGULAR_HPP_INCLUDED__