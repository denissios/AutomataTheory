#include "graph.hpp"

void Graph::process(SynTree& tree)
{
	states.push_back(std::make_shared<State>(tree.root->firstpos));
	for (size_t i = 0; i < states.size(); i++) {
		for (auto& set_elem_1 : states[i]->positions) {
			char c = {};
			bool last_symbol = false;
			size_t count_follow_pos = 0;
			for (auto& [key, value] : tree.followpos) {
				count_follow_pos++;
				if (key.first == set_elem_1) {
					if (count_follow_pos == tree.followpos.size())
						last_symbol = true;
					c = key.second;
					break;
				}
			}

			std::set<size_t> new_state;
			for (auto& set_elem_2 : states[i]->positions) {
				for (auto& [key, value] : tree.followpos) {
					if (key.first == set_elem_2 && key.second == c) {
						new_state.insert(value.begin(), value.end());
					}
				}
			}

			for (size_t j = 0; j < states.size(); j++) {
				if (states[j]->positions == new_state) {
					states[i]->next_states.emplace(c, states[j]);
					break;
				}
				if (j == states.size() - 1) {
					if (c == '&' && last_symbol) {
						states[i]->finite = true;
						states[i]->group = 0;
						break;
					}
					states.push_back(std::make_shared<State>(new_state));
				}
			}
		}
	}
}

void Graph::minimize()
{
	std::vector<size_t> prev(states.size()), cur(states.size());
	for (size_t i = 0; i < states.size(); i++) {
		cur[i] = states[i]->group;
	}
	while (prev != cur) {
		prev = cur;
		for (size_t i = 0; i < states.size(); i++) {
			for (size_t j = i + 1; j < states.size(); j++) {
				if (states[i]->group != states[j]->group)
					continue;
				if (states[i]->next_states.size() != states[j]->next_states.size()) {
					states[j]->group++;
					continue;
				}

				for (auto& [key_i, value_i] : states[i]->next_states) {
					bool is_equal = true, symbol_in_first_and_second = false;
					for (auto& [key_j, value_j] : states[j]->next_states) {
						if (key_i == key_j) {
							symbol_in_first_and_second = true;
							if (value_i->group != value_j->group) {
								states[j]->group++;
								is_equal = false;
								break;
							}
						}
					}

					if (!symbol_in_first_and_second) {
						states[j]->group++;
						is_equal = false;
					}
					if (!is_equal)
						break;
				}
			}
		}
		for (size_t i = 0; i < states.size(); i++) {
			cur[i] = states[i]->group;
		}
	}

	for (size_t i = 0; i < states.size(); i++) {
		for (size_t j = i + 1; j < states.size(); j++) {
			if (states[i]->group == states[j]->group && states[i]->finite == states[j]->finite) {
				states[i]->positions.insert(states[j]->positions.begin(), states[j]->positions.end());
				for (auto& [key, value] : states[j]->next_states) {
					states[i]->next_states.emplace(key, value);
				}
				for (size_t k = 0; k < states.size(); k++) {
					for (auto& [key, value] : states[k]->next_states) {
						if (value->positions == states[j]->positions) {
							value = states[i];
						}
					}
				}
				states.erase(states.begin() + j);
				j--;
			}
		}
	}
}

void Graph::print()
{
	for (size_t i = 0; i < states.size(); i++) {
		printGraph(states[i]);
	}
	std::cout << std::endl << std::endl << std::endl;
}

void Graph::printGraph(std::shared_ptr<State> state)
{
	std::cout << "{ ";
	for (auto& set_elem : state->positions) {
		std::cout << set_elem << " ";
	}
	std::cout << "} " << state->finite << " : ";
	for (auto& [key, value] : state->next_states) {
		std::cout << key << " { ";
		for (auto& set_elem : value->positions) {
			std::cout << set_elem << " ";
		}
		std::cout << "}, ";
	}
	std::cout << std::endl;
}