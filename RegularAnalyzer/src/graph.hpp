#ifndef __GRAPH_HPP_INCLUDED__
#define __GRAPH_HPP_INCLUDED__

#include "synTree.hpp"

class Graph
{
public:
	Graph() : states{} {}
	void process(SynTree& tree);
	void minimize();
	void print();

private:
	struct State
	{
		State(std::set<size_t>& set) : finite{ false }, group{ 1 }
		{
			positions.insert(set.begin(), set.end());
		}

		bool finite;
		size_t group;
		std::set<size_t> positions;
		std::map<char, std::shared_ptr<State>> next_states;
	};

	void printGraph(std::shared_ptr<State> condition);
	
	std::vector<std::shared_ptr<State>> states;
};



#endif //__GRAPH_HPP_INCLUDED__