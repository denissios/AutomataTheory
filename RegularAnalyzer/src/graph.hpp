#ifndef __GRAPH_HPP_INCLUDED__
#define __GRAPH_HPP_INCLUDED__

#include "synTree.hpp"

class Graph
{
public:
	struct State;

	Graph() : states{} {}
	void process(SynTree& tree);
	void minimize();
	std::string getRE();
	bool match(std::string& str, std::vector<std::string>& capture);
	void findAll(std::string& str, std::vector<std::string>& strings, std::vector<std::string>& capture, size_t index);
	void print();
	void multiple_for_sub(Graph& graph1, Graph& graph2);

	bool operator==(Graph& graph);

private:
	struct State
	{
		State(std::set<size_t>& set, size_t number) : is_finite{ false }, group{ 1 }, number{ number }
		{
			positions.insert(set.begin(), set.end());
		}

		bool is_finite;
		size_t number;
		size_t group;
		std::set<size_t> positions;
		std::map<char, std::pair<std::vector<int>, std::shared_ptr<State>>> next_states;
	};


	std::string getREFromGraph(size_t i, size_t j, size_t k);
	std::string GetIState(size_t i, size_t j, size_t k);
	bool checkMeta(const char elem);
	void printGraph(std::shared_ptr<State> condition);
	void multipleNextStates(std::shared_ptr<State> graph1_state, std::shared_ptr<State> graph2_state, std::shared_ptr<State> prev_state, size_t number);
	bool nextStatesEquals(std::shared_ptr<State> graph1_state, std::shared_ptr<State> graph2_state, std::vector<std::set<size_t>> &states_num1, std::vector<std::set<size_t>>& states_num2);

	const std::array<char, 10> META = { '|', '.', '+', '(', ')', '{', '}', '\\', '#', ':' };
	
	std::vector<std::shared_ptr<State>> states;
};



#endif //__GRAPH_HPP_INCLUDED__