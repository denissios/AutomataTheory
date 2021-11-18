#include "graph.hpp"

void Graph::process(SynTree& tree)
{
	if (!tree.root) {
		throw "Empty tree!";
		return;
	}

	size_t number = 1;
	states.push_back(std::make_shared<State>(tree.root->firstpos, number++));
	for (size_t i = 0; i < states.size(); i++) {
		for (auto& set_elem_1 : states[i]->positions) {
			char c = {};
			bool last_symbol = false;
			size_t count_follow_pos = 0;
			std::vector<int> vec;
			for (auto& [key, value] : tree.followpos) {
				count_follow_pos++;
				if (key.first[0] == set_elem_1) {
					if (count_follow_pos == tree.followpos.size())
						last_symbol = true;
					vec = key.first;
					c = key.second;
					break;
				}
			}

			std::set<size_t> new_state;
			for (auto& set_elem_2 : states[i]->positions) {
				for (auto& [key, value] : tree.followpos) {
					if (key.first[0] == set_elem_2 && key.second == c) {
						new_state.insert(value.begin(), value.end());
					}
				}
			}

			for (size_t j = 0; j < states.size(); j++) {
				if (states[j]->positions == new_state) {
					states[i]->next_states.emplace(c, std::make_pair(vec, states[j]));
					break;
				}
				if (j == states.size() - 1) {
					if (c == '&' && last_symbol) {
						states[i]->is_finite = true;
						states[i]->group = 0;
						break;
					}
					states.push_back(std::make_shared<State>(new_state, number++));
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
					cur[j]++;
					//states[j]->group++;
					continue;
				}

				for (auto& [key_i, value_i] : states[i]->next_states) {
					bool is_equal = true, symbol_in_first_and_second = false;
					for (auto& [key_j, value_j] : states[j]->next_states) {
						if (key_i == key_j) {
							symbol_in_first_and_second = true;
							if (value_i.second->group != value_j.second->group || value_i.second->is_finite != value_j.second->is_finite) {
								cur[j]++;
								//states[j]->group++;
								is_equal = false;
								break;
							}
						}
					}

					if (!symbol_in_first_and_second) {
						cur[j]++;
						//states[j]->group++;
						is_equal = false;
					}
					if (!is_equal)
						break;
				}
			}
			for (size_t j = 0; j < states.size(); j++) {
				states[j]->group = cur[j];
			}
		}
		/*for (size_t i = 0; i < states.size(); i++) {
			cur[i] = states[i]->group;
		}*/
	}

	for (size_t i = 0; i < states.size(); i++) {
		for (size_t j = i + 1; j < states.size(); j++) {
			if (states[i]->group == states[j]->group && states[i]->is_finite == states[j]->is_finite) {
				states[i]->positions.insert(states[j]->positions.begin(), states[j]->positions.end());
				for (size_t k = 0; k < states.size(); k++) {
					for (auto& [key, value] : states[k]->next_states) {
						if (value.second->positions == states[j]->positions) {
							value.second = states[i];
						}
					}
				}

				for (auto& [key_i, value_i] : states[i]->next_states) {
					for (auto& [key_j, value_j] : states[j]->next_states) {
						if (key_i == key_j) {
							value_i.first[0] = value_j.first[0];
							if (value_j.first[1] != 0 && value_i.first[1] == 0) {
								value_i.first[1] = value_j.first[1];
								value_i.first[2] = value_j.first[2];
								value_i.first[3] = value_j.first[3];
							}
							break;
						}
					}
				}

				states.erase(states.begin() + j);
				for (size_t k = j; k < states.size(); k++) {
					states[k]->number--;
				}
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
	std::cout << "} " << state->is_finite << " : ";
	for (auto& [key, value] : state->next_states) {
		std::cout << key << " { ";
		for (auto& set_elem : value.second->positions) {
			std::cout << set_elem << " ";
		}
		std::cout << "}, ";
	}
	std::cout << std::endl;
}

std::string Graph::getRE()
{
	std::string str_res;
	for (size_t j = 0, iteration = 0; j < states.size(); j++) {
		if (states[j]->is_finite) {
			if (iteration)
				str_res += "|";
			iteration++;
			std::string str = getREFromGraph(1, j + 1, states.size());

			for (size_t k = 0; k < str.size(); k++) {
				if (k != str.size() - 1 && str[k] == '(' && str[k + 1] == ')') {
					if (k != str.size() - 2 && str[k + 2] == '{') {
						str.erase(str.begin() + k + 2, str.begin() + k + 6);
					}
					str.erase(str.begin() + k, str.begin() + k + 2);
				}
				else if (k != str.size() - 1 && str[k] == '|' && str[k + 1] == '|') {
					str.erase(str.begin() + k, str.begin() + k + 2);
					k--;
				}
			}
			str_res += str;
		}
	}

	return str_res;
}

std::string Graph::getREFromGraph(size_t i, size_t j, size_t k)
{
	std::string str_result;
	if (k) {
		std::string tmp1 = "(" + getREFromGraph(i, j, k - 1);
		std::string tmp2 = getREFromGraph(i, k, k - 1);
		std::string tmp3 = "(" + getREFromGraph(k, k, k - 1) + "){0,}";
		std::string tmp4 = getREFromGraph(k, j, k - 1);

		if (tmp1 != "(") {
			str_result += tmp1;
			if (tmp2 != "" && tmp4 != "") {
				str_result += "|" + tmp2 + tmp3 + tmp4;
			}
			str_result += ")";
		}
		else if (tmp2 != "" && tmp4 != "") {
			str_result += "(" + tmp2 + tmp3 + tmp4 + ")";
		}
		else str_result = "";
		
		
		return str_result == "()" ? "" : str_result;
	}
	
	std::string tmp;
	tmp = GetIState(i, j, k);
	if (tmp.size() && tmp[tmp.size() - 1] == '|') {
		while (tmp[tmp.size() - 1] == '|') {
			tmp.erase(tmp.end() - 1);
		}
	}

	return tmp;
}

std::string Graph::GetIState(size_t i, size_t j, size_t k)
{
	size_t iteration = 0;
	std::string str;
	if (i == j) {
		str = "^|";
	}
	for (auto& [key, value] : states[i - 1]->next_states) {
		if (iteration && str != "")
			str += "|";
		iteration++;
		if (value.second->number == states[j - 1]->number) {
			str += checkMeta(key) ? "#" + std::string(1, key) : std::string(1, key);
		}
	}

	return str;
}

bool Graph::checkMeta(const char elem)
{
	return std::find(std::begin(META), std::end(META), elem) != std::end(META);
}

bool Graph::match(std::string& str, std::vector<std::string>& capture)
{
	if (!str.size() && states.size() == 1 && states[0]->is_finite) {
		return true;
	}

	std::shared_ptr<State> cur_state = states[0];
	std::string str_capture, str_capture_repeat;
	int num_capture = -1, num_capture_repeat = -1;
	for (size_t i = 0; i < str.size(); i++) {
		bool equal = false;
		for (auto& [key, value] : cur_state->next_states) {
			if (key == str[i]) {
				if (i == str.size() - 1) {
					if (value.first[3] != num_capture) {  // for capture
						if (str_capture != "") {
							capture.push_back(str_capture);
							str_capture = "";
						}
						if (value.first[3] != -1) {
							num_capture = value.first[3];
							str_capture += str[i];
							capture.push_back(str_capture);
						}
					}
					else if (str_capture != "" && value.first[3] == num_capture) {
						str_capture += str[i];
						capture.push_back(str_capture);
						str_capture = "";
					}

					if (value.first[2] != num_capture_repeat) {  // for capture_repeat
						if (str_capture_repeat != "") {
							if (std::find_if(capture.begin(), capture.end(), [&str_capture_repeat](std::string& str) {return str == str_capture_repeat; }) == capture.end()) {
								capture = {};
								return false;
							}
							str_capture_repeat = "";
						}
						if (value.first[2] != -1) {
							num_capture_repeat = value.first[2];
							str_capture_repeat += str[i];
							if (std::find_if(capture.begin(), capture.end(), [&str_capture_repeat](std::string& str) {return str == str_capture_repeat; }) == capture.end()) {
								capture = {};
								return false;
							}
						}
					}
					else if (str_capture_repeat != "" && value.first[2] == num_capture_repeat) {
						str_capture_repeat += str[i];
						if (std::find_if(capture.begin(), capture.end(), [&str_capture_repeat](std::string& str) {return str == str_capture_repeat; }) == capture.end()) {
							capture = {};
							return false;
						}
						str_capture_repeat = "";
					}
				}
				else {
					if (str_capture != "" && value.first[3] != num_capture) {
						capture.push_back(str_capture);
						str_capture = "";
					}
					if (str_capture_repeat != "" && value.first[2] != num_capture_repeat) {
						if (std::find_if(capture.begin(), capture.end(), [&str_capture_repeat](std::string& str) {return str == str_capture_repeat; }) == capture.end()) {
							capture = {};
							return false;
						}
						str_capture_repeat = "";
					}
				}
				if (value.first[1] == 1) {  // if capture was found
					num_capture = value.first[3];
					str_capture += str[i];
				}

				if (value.first[1] == 2) {  // if capture_repeat was found
					num_capture_repeat = value.first[2];
					str_capture_repeat += str[i];
				}

				cur_state = value.second;
				equal = true;

				if (value.second->is_finite && i == str.size() - 1) {
					return true;
				}
			}
		}

		if (!equal) {
			capture = {};
			return false;
		}
	}

	capture = {};
	return false;
}

void Graph::findAll(std::string& str, std::vector<std::string>& strings, std::vector<std::string>& capture, size_t index)
{
	std::shared_ptr<State> cur_state = states[0];
	std::string str_capture, str_capture_repeat, current_string;
	std::vector<std::string> captures;
	int num_capture = -1, num_capture_repeat = -1;
	for (size_t i = index; i < str.size(); i++) {
		bool equal = false;
		for (auto& [key, value] : cur_state->next_states) {
			if (key == str[i]) {
				if (i == str.size() - 1) {
					if (value.first[3] != num_capture) {  // for capture
						if (str_capture != "") {
							captures.push_back(str_capture);
							str_capture = "";
						}
						if (value.first[3] != -1) {
							num_capture = value.first[3];
							str_capture += str[i];
							captures.push_back(str_capture);
						}
					}
					else if (str_capture != "" && value.first[3] == num_capture) {
						str_capture += str[i];
						captures.push_back(str_capture);
						str_capture = "";
					}

					if (value.first[2] != num_capture_repeat) {  // for capture_repeat
						if (str_capture_repeat != "") {
							if (std::find_if(capture.begin(), capture.end(), [&str_capture_repeat](std::string& str) {return str == str_capture_repeat; }) == capture.end()) {
								bool find_capture_flag = false;
								for (size_t i = 0; i < captures.size(); i++) {
									if (captures[i] == str_capture_repeat) {
										find_capture_flag = true;
									}
								}
								if (!find_capture_flag) {
									findAll(str, strings, capture, i);
									return;
								}
							}
							str_capture_repeat = "";
						}
						if (value.first[2] != -1) {
							num_capture_repeat = value.first[2];
							str_capture_repeat += str[i];
							if (std::find_if(capture.begin(), capture.end(), [&str_capture_repeat](std::string& str) {return str == str_capture_repeat; }) == capture.end()) {
								bool find_capture_flag = false;
								for (size_t i = 0; i < captures.size(); i++) {
									if (captures[i] == str_capture_repeat) {
										find_capture_flag = true;
									}
								}
								if (!find_capture_flag) {
									findAll(str, strings, capture, i);
									return;
								}
							}
						}
					}
					else if (str_capture_repeat != "" && value.first[2] == num_capture_repeat) {
						str_capture_repeat += str[i];
						if (std::find_if(capture.begin(), capture.end(), [&str_capture_repeat](std::string& str) {return str == str_capture_repeat; }) == capture.end()) {
							bool find_capture_flag = false;
							for (size_t i = 0; i < captures.size(); i++) {
								if (captures[i] == str_capture_repeat) {
									find_capture_flag = true;
								}
							}
							if (!find_capture_flag) {
								findAll(str, strings, capture, i);
								return;
							}
						}
						str_capture_repeat = "";
					}
				}
				else {
					if (str_capture != "" && value.first[3] != num_capture) {
						captures.push_back(str_capture);
						str_capture = "";
					}
					if (str_capture_repeat != "" && value.first[2] != num_capture_repeat) {
						if (std::find_if(capture.begin(), capture.end(), [&str_capture_repeat](std::string& str) {return str == str_capture_repeat; }) == capture.end()) {
							bool find_capture_flag = false;
							for (size_t i = 0; i < captures.size(); i++) {
								if (captures[i] == str_capture_repeat) {
									find_capture_flag = true;
								}
							}
							if (!find_capture_flag) {
								findAll(str, strings, capture, i);
								return;
							}
						}
						str_capture_repeat = "";
					}
				}
				if (value.first[1] == 1) {  // if capture was found
					num_capture = value.first[3];
					str_capture += str[i];
				}

				if (value.first[1] == 2) {  // if capture_repeat was found
					num_capture_repeat = value.first[2];
					str_capture_repeat += str[i];
				}

				current_string += str[i];
				cur_state = value.second;
				equal = true;

				if (value.second->is_finite && i == str.size() - 1) {
					strings.push_back(current_string);
					for (size_t z = 0; z < captures.size(); z++) {
						capture.push_back(captures[z]);
					}
					findAll(str, strings, capture, i + 1);
					return;
				}
			}
		}
		if (!equal && cur_state->is_finite) {
			if (str_capture_repeat != "") {
				if (std::find_if(capture.begin(), capture.end(), [&str_capture_repeat](std::string& str) {return str == str_capture_repeat; }) == capture.end()) {
					bool find_capture_flag = false;
					for (size_t i = 0; i < captures.size(); i++) {
						if (captures[i] == str_capture_repeat) {
							find_capture_flag = true;
						}
					}
					if (!find_capture_flag) {
						findAll(str, strings, capture, i);
						return;
					}
				}
				str_capture_repeat = "";
			}
			if (str_capture != "") {
				captures.push_back(str_capture);
				str_capture = "";
			}
			for (size_t z = 0; z < captures.size(); z++) {
				capture.push_back(captures[z]);
			}
			strings.push_back(current_string);
			if (i == index) i++;
			findAll(str, strings, capture, i);
			return;
		}
		if (!equal) {
			if (i == index) i++;
			findAll(str, strings, capture, i);
			return;
		}
	}
}

void Graph::multiple_for_sub(Graph& graph1, Graph& graph2)
{
	size_t number = 1;
	std::set<size_t> new_state_set;
	new_state_set.insert(graph1.states[0]->positions.begin(), graph1.states[0]->positions.end());
	new_state_set.insert(graph2.states[0]->positions.begin(), graph2.states[0]->positions.end());
	states.push_back(std::make_shared<State>(new_state_set, number++));
	if (graph1.states[0]) {
		for (auto& [key_1, value_1] : graph1.states[0]->next_states) {
			bool is_exist = false;
			if (graph2.states[0]) {
				for (auto& [key_2, value_2] : graph2.states[0]->next_states) {
					if (key_1 == key_2) {
						is_exist = true;
						new_state_set = {};
						new_state_set.insert(value_1.second->positions.begin(), value_1.second->positions.end());
						new_state_set.insert(value_2.second->positions.begin(), value_2.second->positions.end());
						std::vector<int> vec = { value_2.first[0] };
						if (value_2.first[1] != 0 && value_1.first[1] == 0) {
							vec.push_back(value_2.first[1]);
							vec.push_back(value_2.first[2]);
							vec.push_back(value_2.first[3]);
						}
						else {
							vec.push_back(value_1.first[1]);
							vec.push_back(value_1.first[2]);
							vec.push_back(value_1.first[3]);
						}
						for (size_t i = 0; i < states.size(); i++) {
							if (states[i]->positions == new_state_set) {
								states[0]->next_states.emplace(key_1, std::make_pair(vec, states[i]));
								break;
							}
							if (i == states.size() - 1) {
								states.push_back(std::make_shared<State>(new_state_set, number++));
								states[0]->next_states.emplace(key_1, std::make_pair(vec, states[states.size() - 1]));
								if (value_1.second->is_finite && !value_2.second->is_finite) {
									states[states.size() - 1]->is_finite = true;
								}
								multipleNextStates(value_1.second, value_2.second, states[states.size() - 1], number);
								break;
							}
						}
					}
				}
			}
			if (!is_exist) {
				new_state_set = {};
				new_state_set.insert(value_1.second->positions.begin(), value_1.second->positions.end());
				std::vector<int> vec = value_1.first;
				for (size_t i = 0; i < states.size(); i++) {
					if (states[i]->positions == new_state_set) {
						states[0]->next_states.emplace(key_1, std::make_pair(vec, states[i]));
						break;
					}
					if (i == states.size() - 1) {
						states.push_back(std::make_shared<State>(new_state_set, number++));
						states[0]->next_states.emplace(key_1, std::make_pair(vec, states[states.size() - 1]));
						if (value_1.second->is_finite) {
							states[states.size() - 1]->is_finite = true;
						}
						multipleNextStates(value_1.second, nullptr, states[states.size() - 1], number);
						break;
					}
				}
			}
		}
	}

	if (graph2.states[0]) {
		for (auto& [key_2, value_2] : graph2.states[0]->next_states) {
			bool is_exist = false;
			if (graph1.states[0]) {
				for (auto& [key_1, value_1] : graph1.states[0]->next_states) {
					if (key_1 == key_2) {
						is_exist = true;
						break;
					}
				}
			}
			if (!is_exist) {
				new_state_set = { 0 };
				new_state_set.insert(value_2.second->positions.begin(), value_2.second->positions.end());
				std::vector<int> vec = value_2.first;
				for (size_t i = 0; i < states.size(); i++) {
					if (states[i]->positions == new_state_set) {
						states[0]->next_states.emplace(key_2, std::make_pair(vec, states[i]));
						break;
					}
					if (i == states.size() - 1) {
						states.push_back(std::make_shared<State>(new_state_set, number++));
						states[0]->next_states.emplace(key_2, std::make_pair(vec, states[states.size() - 1]));
						multipleNextStates(nullptr, value_2.second, states[states.size() - 1], number);
						break;
					}
				}
			}
		}
	}
}

void Graph::multipleNextStates(std::shared_ptr<State> graph1_state, std::shared_ptr<State> graph2_state, std::shared_ptr<State> prev_state, size_t number)
{
	if (graph1_state) {
		for (auto& [key_1, value_1] : graph1_state->next_states) {
			bool is_exist = false;
			if (graph2_state) {
				for (auto& [key_2, value_2] : graph2_state->next_states) {
					if (key_1 == key_2) {
						is_exist = true;
						std::set<size_t> new_state_set;
						new_state_set.insert(value_1.second->positions.begin(), value_1.second->positions.end());
						new_state_set.insert(value_2.second->positions.begin(), value_2.second->positions.end());
						std::vector<int> vec = { value_2.first[0] };
						if (value_2.first[1] != 0 && value_1.first[1] == 0) {
							vec.push_back(value_2.first[1]);
							vec.push_back(value_2.first[2]);
							vec.push_back(value_2.first[3]);
						}
						else {
							vec.push_back(value_1.first[1]);
							vec.push_back(value_1.first[2]);
							vec.push_back(value_1.first[3]);
						}
						for (size_t i = 0; i < states.size(); i++) {
							if (states[i]->positions == new_state_set) {
								prev_state->next_states.emplace(key_1, std::make_pair(vec, states[i]));
								break;
							}
							if (i == states.size() - 1) {
								states.push_back(std::make_shared<State>(new_state_set, number++));
								prev_state->next_states.emplace(key_1, std::make_pair(vec, states[states.size() - 1]));
								if (value_1.second->is_finite && !value_2.second->is_finite) {
									states[states.size() - 1]->is_finite = true;
								}
								multipleNextStates(value_1.second, value_2.second, states[states.size() - 1], number);
								break;
							}
						}
					}
				}
				
			}
			if (!is_exist) {
				std::set<size_t> new_state_set = {};
				new_state_set.insert(value_1.second->positions.begin(), value_1.second->positions.end());
				std::vector<int> vec = value_1.first;
				for (size_t i = 0; i < states.size(); i++) {
					if (states[i]->positions == new_state_set) {
						states[0]->next_states.emplace(key_1, std::make_pair(vec, states[i]));
						break;
					}
					if (i == states.size() - 1) {
						states.push_back(std::make_shared<State>(new_state_set, number++));
						prev_state->next_states.emplace(key_1, std::make_pair(vec, states[states.size() - 1]));
						if (value_1.second->is_finite) {
							states[states.size() - 1]->is_finite = true;
						}
						multipleNextStates(value_1.second, nullptr, states[states.size() - 1], number);
						break;
					}
				}
			}
		}
	}

	if(graph2_state) {
		for (auto& [key_2, value_2] : graph2_state->next_states) {
			bool is_exist = false;
			if (graph1_state) {
				for (auto& [key_1, value_1] : graph1_state->next_states) {
					if (key_1 == key_2) {
						is_exist = true;
						break;
					}
				}
			}
			if (!is_exist) {
				std::set<size_t> new_state_set = { 0 };
				new_state_set.insert(value_2.second->positions.begin(), value_2.second->positions.end());
				std::vector<int> vec = value_2.first;
				for (size_t i = 0; i < states.size(); i++) {
					if (states[i]->positions == new_state_set) {
						prev_state->next_states.emplace(key_2, std::make_pair(vec, states[i]));
						break;
					}
					if (i == states.size() - 1) {
						states.push_back(std::make_shared<State>(new_state_set, number++));
						prev_state->next_states.emplace(key_2, std::make_pair(vec, states[states.size() - 1]));
						multipleNextStates(nullptr, value_2.second, states[states.size() - 1], number);
						break;
					}
				}
			}
		}
	}
}

bool Graph::operator==(Graph& graph)
{
	if (this->states.size() != graph.states.size()) {
		return false;
	}

	if (this->states[0]->next_states.size() != graph.states[0]->next_states.size()) {
		return false;
	}

	std::vector<std::set<size_t>> states_num1, states_num2;
	states_num1.push_back(this->states[0]->positions);
	states_num2.push_back(graph.states[0]->positions);
	for (auto& [key1, value1] : this->states[0]->next_states) {
		bool is_exist = false;
		for (auto& [key2, value2] : graph.states[0]->next_states) {
			if (key1 == key2) {
				if (std::find(states_num1.begin(), states_num1.end(), value1.second->positions) == states_num1.end() && std::find(states_num2.begin(), states_num2.end(), value2.second->positions) != states_num2.end() ||
					std::find(states_num1.begin(), states_num1.end(), value1.second->positions) != states_num1.end() && std::find(states_num2.begin(), states_num2.end(), value2.second->positions) == states_num2.end()) {
					return false;
				}

				is_exist = true;
				if (std::find(states_num1.begin(), states_num1.end(), value1.second->positions) == states_num1.end() && std::find(states_num2.begin(), states_num2.end(), value2.second->positions) == states_num2.end()) {
					states_num1.push_back(value1.second->positions);
					states_num2.push_back(value2.second->positions);
					
					if (!nextStatesEquals(value1.second, value2.second, states_num1, states_num2)) {
						return false;
					}
				}
			}
		}

		if (!is_exist) {
			return false;
		}
	}

	return true;
}

bool Graph::nextStatesEquals(std::shared_ptr<State> graph1_state, std::shared_ptr<State> graph2_state, std::vector<std::set<size_t>>& states_num1, std::vector<std::set<size_t>>& states_num2)
{
	if (graph1_state->next_states.size() != graph2_state->next_states.size()) {
		return false;
	}

	for (auto& [key1, value1] : graph1_state->next_states) {
		bool is_exist = false;
		for (auto& [key2, value2] : graph2_state->next_states) {
			if (key1 == key2) {
				if (std::find(states_num1.begin(), states_num1.end(), value1.second->positions) == states_num1.end() && std::find(states_num2.begin(), states_num2.end(), value2.second->positions) != states_num2.end() ||
					std::find(states_num1.begin(), states_num1.end(), value1.second->positions) != states_num1.end() && std::find(states_num2.begin(), states_num2.end(), value2.second->positions) == states_num2.end()) {
					return false;
				}

				is_exist = true;
				if (std::find(states_num1.begin(), states_num1.end(), value1.second->positions) == states_num1.end() && std::find(states_num2.begin(), states_num2.end(), value2.second->positions) == states_num2.end()) {
					states_num1.push_back(value1.second->positions);
					states_num2.push_back(value2.second->positions);

					if (!nextStatesEquals(value1.second, value2.second, states_num1, states_num2)) {
						return false;
					}
				}
			}
		}

		if (!is_exist) {
			return false;
		}
	}

	return true;
}