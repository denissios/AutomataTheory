#include "synTree.hpp"

void SynTree::process()
{
	if (regular == "") {
		return;
	}

	
	regular.insert(0, "(");
	regular += "&)";
	for (size_t i = 0; i < regular.size(); i++) { // search '(' and ')'
		if (regular[i] == '(') {
			size_t j = i, count_pound = 0;
			while (j > 0 && regular[j - 1] == '#') {
				count_pound++;
				j--;
			}
			if(count_pound % 2 == 0)
				left_brackets.push_back(i);
		}
			
		if (regular[i] == ')') {
			size_t j = i, count_pound = 0;
			while (j > 0 && regular[j - 1] == '#') {
				count_pound++;
				j--;
			}
			if (count_pound % 2 == 0) {
				if (left_brackets.empty()) {
					throw "Invalid '(' or ')'";
					return;
				}
				right_brackets.push_back(i);
				bracketHandler(left_brackets[left_brackets.size() - 1], i);

				regular.erase(regular.begin() + left_brackets[left_brackets.size() - 1] + 1, regular.begin() + i + 1);
				right_brackets[right_brackets.size() - 1] -= (i - left_brackets[left_brackets.size() - 1]);
				i -= (i - left_brackets[left_brackets.size() - 1]);
				left_brackets.erase(left_brackets.end() - 1);
			}
		}
	}
	if (!left_brackets.empty()) {
		throw "Invalid '(' or ')'";
		return;
	}

	root = nodes[0];
}

void SynTree::bracketHandler(size_t first, size_t last)
{
	createNodes(first, last);
	getFLErrors();

	nodes.erase(std::remove_if(nodes.begin(), nodes.end(), [](const std::shared_ptr<Node>& node) { return (node->c == '#' && node->escaping == false); }), nodes.end());  // delete all not escaping '#'

	std::string str_capture = {};
	bool capture_flag = false;
	captureHandler(str_capture, capture_flag, first);

	rangeHandler();
	concatinationHandler();
	orHandler();

	if (capture_flag) {
		for (auto& [key, value] : capture) {
			if (key == std::stoi(str_capture)) {
				throw "Number of capture repeated more than once";
				return;
			}
		}
		capture.emplace(std::stoi(str_capture), nodes[num_of_brackets_in_nodes]);
	}

	num_of_brackets_in_nodes++;
}

void SynTree::createNodes(size_t first, size_t last)
{
	for (size_t i = first + 1; i < last; i++) {  // create node for all symbols in this ()
		size_t j = i, count_pound = 0;
		while (j > first && regular[j - 1] == '#') {
			count_pound++;
			j--;
		}
		if (count_pound % 2 == 0) {
			nodes.push_back(std::make_shared<Node>(regular[i], false));
		}
		else {
			nodes.push_back(std::make_shared<Node>(regular[i], true));
		}
	}
}

void SynTree::captureHandler(std::string& str_capture, bool& capture_flag, size_t& first)
{
	bool true_capture_flag = true;
	size_t count_capture = 0;
	for (size_t i = num_of_brackets_in_nodes; i < nodes.size(); i++) {  // search "n:"
		if (nodes[i]->c == ':' && !nodes[i]->escaping && capture_flag) {  // invalid ":"
			throw "Invalid ':'";
			return;
		}

		if (nodes[i]->c == ':' && !nodes[i]->escaping) {
			if (!true_capture_flag || str_capture == "") {
				throw "Invalid capture n:";
				return;
			}
			capture_flag = true;
		}

		if ((nodes[i]->c > '9' || nodes[i]->c < '0') && !capture_flag)
			true_capture_flag = false;

		if (nodes[i]->c == '(' && !nodes[i]->escaping && !nodes[i]->is_root_subtree) {
			for (size_t j = 0; j < right_brackets.size(); j++) {
				if (first < right_brackets[j]) {
					nodes[i] = nodes[j];
					right_brackets.erase(right_brackets.begin() + j);
					nodes.erase(nodes.begin() + j);
					break;
				}
			}
			//copyNode(0, i);
			num_of_brackets_in_nodes--;
			i--;
		}

		if (!capture_flag) {
			count_capture++;
			str_capture += nodes[i]->c;
		}

		if (nodes[i]->c == '\\' && !nodes[i]->escaping) { // search "\n"
			size_t j = i + 1, count = 0;
			std::string str_num_capture = {};
			while (j < nodes.size() && nodes[j]->c > '0' && nodes[j]->c < '9') {
				str_num_capture += nodes[j]->c;
				j++;
				count++;
			}
			if (j == i + 1) {
				throw "Invalid capture \\n";
				return;
			}
			bool is_exist_capture = false;
			for (auto& [key, value] : capture) {
				if (key == std::stoi(str_num_capture)) {
					nodes[i] = copyTree(value, nullptr);
					is_exist_capture = true;
					break;
				}
			}
			if (!is_exist_capture) {
				throw "Number of capture doesn't exist";
				return;
			}
			nodes.erase(nodes.begin() + i + 1, nodes.begin() + i + count + 1);
		}

		if (i == nodes.size() - 1 && true_capture_flag)
			nodes.erase(nodes.begin() + num_of_brackets_in_nodes, nodes.begin() + num_of_brackets_in_nodes + count_capture + 1);
	}
}

void SynTree::rangeHandler()
{
	for (size_t i = num_of_brackets_in_nodes; i < nodes.size(); i++) {
		if (nodes[i]->c == '{' && !nodes[i]->escaping) {  // search '{'
			if (i == num_of_brackets_in_nodes) {
				throw "Nothing before '{'";
				return;
			}
			if (i > num_of_brackets_in_nodes && checkMeta(nodes[i - 1]->c, 0) && !nodes[i - 1]->escaping && !nodes[i - 1]->is_root_subtree) {
				throw "Invalid '|' or '.' before '{'";
				return;
			}
			nodes.erase(nodes.begin() + i);
			std::string str = "";
			size_t j = i, count = 0, x = 0, y = 0;
			bool coma_flag = false;
			while (true) {
				if (j == nodes.size() - 1 && !(nodes[j]->c == '}' && !nodes[j]->escaping && !nodes[j]->is_root_subtree)) {
					throw "Can't find '}'";
					return;
				}
				count++;
				if (nodes[j]->c == ',' && !nodes[j]->escaping && !nodes[j]->is_root_subtree) {
					coma_flag = true;
					if (nodes[j + 1]->c == '}') {
						count++;
						nodes.erase(nodes.begin() + i, nodes.begin() + i + count);
						x = std::stoi(str);
						oneNumRangeHandler(x, i);
						break;
					}
					else {
						x = std::stoi(str);
						str = "";
						j++;
						continue;
					}
				}

				if (nodes[j]->c == '}' && !nodes[j]->escaping && !nodes[j]->is_root_subtree) {
					if (!coma_flag || x > std::stoi(str)) {
						throw "Invalid range {}";
						return;
					}
					y = std::stoi(str);
					nodes.erase(nodes.begin() + i, nodes.begin() + i + count);
					twoNumRangeHandler(x, y, i);
					break;
				}

				if (nodes[j]->c > '9' || nodes[j]->c < '0') {
					throw "Invalid range {}";
					return;
				}

				str += nodes[j]->c;
				j++;
			}
		}
		if (nodes[i]->c == '}') {
			throw "Can't find '{'";
			return;
		}

		if (nodes[i]->c == '+' && !nodes[i]->escaping && !nodes[i]->is_root_subtree) {  // search '+'
			if (i > num_of_brackets_in_nodes && checkMeta(nodes[i - 1]->c, 0) && !nodes[i - 1]->escaping && !nodes[i - 1]->is_root_subtree) {
				throw "Invalid '|' or '.' before '+'";
				return;
			}
			nodes[i]->left = nodes[i - 1];
			nodes[i]->is_root_subtree = true;
			nodes[i - 1]->parent = nodes[i];
			nodes.erase(nodes.begin() + i - 1);
			i--;
		}
	}
}

void SynTree::oneNumRangeHandler(size_t& x, size_t& i)
{
	if (!x) {
		nodes.insert(nodes.begin() + i, std::make_shared<Node>('+', false, true));
		nodes[i]->left = nodes[i - 1];
		nodes[i]->is_root_subtree = true;
		nodes[i - 1]->parent = nodes[i];
		nodes.erase(nodes.begin() + i - 1);
		nodes.insert(nodes.begin() + i, std::make_shared<Node>('^', false, true));
		nodes.insert(nodes.begin() + i, std::make_shared<Node>('|', false, true));
		nodes[i]->left = nodes[i - 1];
		nodes[i]->right = nodes[i + 1];
		nodes[i]->is_root_subtree = true;
		nodes[i - 1]->parent = nodes[i];
		nodes[i + 1]->parent = nodes[i];
		nodes.erase(nodes.begin() + i + 1);
		nodes.erase(nodes.begin() + i - 1);
		return;
	}

	std::shared_ptr<Node> tmp = copyTree(nodes[i - 1], nullptr);
	for (size_t k = 0; k < x; k++) {
		if (k == x - 1) {
			nodes.insert(nodes.begin() + i, std::make_shared<Node>('+', false, true));
			nodes[i]->left = nodes[i - 1];
			nodes[i]->is_root_subtree = true;
			nodes[i - 1]->parent = nodes[i];
			nodes.erase(nodes.begin() + i - 1);
			i--;
			break;
		}
		nodes.insert(nodes.begin() + i, copyTree(tmp, nullptr));
		nodes.insert(nodes.begin() + i, std::make_shared<Node>('.', false));
		nodes[i]->left = nodes[i - 1];
		nodes[i]->right = nodes[i + 1];
		nodes[i]->is_root_subtree = true;
		nodes[i - 1]->parent = nodes[i];
		nodes[i + 1]->parent = nodes[i];
		nodes.erase(nodes.begin() + i - 1);
		nodes.erase(nodes.begin() + i);
	}
}

void SynTree::twoNumRangeHandler(size_t& x, size_t& y, size_t& i)
{
	if (!y) {
		i--;
		nodes.erase(nodes.begin() + i);
		return;
	}

	std::shared_ptr<Node> tmp = copyTree(nodes[i - 1], nullptr);
	for (size_t z = 0; z <= y - x; z++) {
		for (size_t k = 0; k < x + z; k++) {
			if (k == x + z - 1) {
				if (z) {
					i -= 2;
					nodes[i]->left = nodes[i - 1];
					nodes[i]->right = nodes[i + 1];
					nodes[i]->is_root_subtree = true;
					nodes[i - 1]->parent = nodes[i];
					nodes[i + 1]->parent = nodes[i];
					nodes.erase(nodes.begin() + i + 1);
					nodes.erase(nodes.begin() + i - 1);
				}
				if (z == y - x)
					break;
				nodes.insert(nodes.begin() + i, copyTree(tmp, nullptr));
				nodes.insert(nodes.begin() + i, std::make_shared<Node>('|', false, true));
				i += 2;
				break;
			}
			nodes.insert(nodes.begin() + i, copyTree(tmp, nullptr));
			nodes.insert(nodes.begin() + i, std::make_shared<Node>('.', false));
			nodes[i]->left = nodes[i - 1];
			nodes[i]->right = nodes[i + 1];
			nodes[i]->is_root_subtree = true;
			nodes[i - 1]->parent = nodes[i];
			nodes[i + 1]->parent = nodes[i];
			nodes.erase(nodes.begin() + i - 1);
			nodes.erase(nodes.begin() + i);
		}
	}
}

void SynTree::concatinationHandler()
{
	for (size_t i = num_of_brackets_in_nodes + 1; i < nodes.size(); i++) {  // search concatination
		getErrors(i);
		if (nodes[i]->c == '.' && !nodes[i]->escaping && !nodes[i]->is_root_subtree) {
			nodes[i]->left = nodes[i - 1];
			nodes[i]->right = nodes[i + 1];
			nodes[i]->is_root_subtree = true;
			nodes[i - 1]->parent = nodes[i];
			nodes[i + 1]->parent = nodes[i];
			nodes.erase(nodes.begin() + i - 1);
			nodes.erase(nodes.begin() + i);
			i--;
		}
		else if ((nodes[i]->c != '|') || (nodes[i]->c == '|' && nodes[i]->escaping) || (nodes[i]->c == '|' && nodes[i]->is_root_subtree)) {
			if (nodes[i - 1]->c == '|' && !nodes[i - 1]->escaping && !nodes[i - 1]->is_root_subtree)
				continue;

			nodes.insert(nodes.begin() + i, std::make_shared<Node>('.', false));
			nodes[i]->left = nodes[i - 1];
			nodes[i]->right = nodes[i + 1];
			nodes[i]->is_root_subtree = true;
			nodes[i - 1]->parent = nodes[i];
			nodes[i + 1]->parent = nodes[i];
			nodes.erase(nodes.begin() + i + 1);
			nodes.erase(nodes.begin() + i - 1);
			i--;
		}
	}
}

void SynTree::orHandler()
{
	for (size_t i = num_of_brackets_in_nodes; i < nodes.size(); i++) {  // search '|'
		if (nodes[i]->c == '|' && !nodes[i]->escaping && !nodes[i]->is_root_subtree) {
			nodes[i]->left = nodes[i - 1];
			nodes[i]->right = nodes[i + 1];
			nodes[i]->is_root_subtree = true;
			nodes[i - 1]->parent = nodes[i];
			nodes[i + 1]->parent = nodes[i];
			nodes.erase(nodes.begin() + i + 1);
			nodes.erase(nodes.begin() + i - 1);
			i--;
		}
	}
}

std::shared_ptr<SynTree::Node> SynTree::copyTree(std::shared_ptr<Node> tree_c, std::shared_ptr<Node> cur_parent)
{
	if (!tree_c)
		return nullptr;

	auto new_node = std::make_shared<Node>(tree_c->c, tree_c->escaping, tree_c->is_root_subtree);
	new_node->parent = cur_parent;
	new_node->left = copyTree(tree_c->left, new_node);
	new_node->right = copyTree(tree_c->right, new_node);

	return new_node;
}

void SynTree::getErrors(const size_t idx)
{
	if (idx < nodes.size() - 1 && checkMeta(nodes[idx + 1]->c, 0) && !nodes[idx + 1]->escaping && !nodes[idx + 1]->is_root_subtree && checkMeta(nodes[idx]->c, 0) && !nodes[idx]->escaping && !nodes[idx]->is_root_subtree) {
		throw "Invalid '|' or '.' or '+'";
		return;
	}
}

void SynTree::getFLErrors()
{
	if (checkMeta(nodes[num_of_brackets_in_nodes]->c, 0) && !nodes[num_of_brackets_in_nodes]->escaping && !nodes[num_of_brackets_in_nodes]->is_root_subtree ||
		checkMeta(nodes[nodes.size() - 1]->c, 1) && !nodes[nodes.size() - 1]->escaping && !nodes[nodes.size() - 1]->is_root_subtree) {
		throw "Invalid '|' or '.' or '+'";
		return;
	}
}

bool SynTree::checkMeta(const char elem, const size_t end)
{
	return std::find(std::begin(META), std::end(META) - end, elem) != std::begin(META) + 3 - end;
}

void SynTree::print()
{
	printTree(root, 0);
	std::cout << std::endl << std::endl << std::endl;
	printNFLTree(root, 0);
	std::cout << std::endl << std::endl << std::endl;
	for (auto& [key, value] : followpos) {
		std::cout << key.first << " " << key.second << " ";
		std::cout << "{ ";
		for (auto& el : value) {
			std::cout << el << " ";
		}
		std::cout << "}";
		std::cout << std::endl;
	}
	std::cout << std::endl << std::endl << std::endl;
}

void SynTree::printTree(std::shared_ptr<Node> m_node, size_t level)
{
	if (m_node)
	{
		printTree(m_node->left, level + 1);
		for (size_t i = 0; i < level; i++) std::cout << "   ";
		std::cout << m_node->c << std::endl;
		printTree(m_node->right, level + 1);
	}
}

void SynTree::printNFLTree(std::shared_ptr<Node> m_node, size_t level)
{
	if (m_node)
	{
		printNFLTree(m_node->left, level + 1);
		for (size_t i = 0; i < level; i++) std::cout << "         ";
		std::cout << m_node->nullable << " {";
		for (auto& el : m_node->firstpos) {
			std::cout << el << " ";
		}
		std::cout << "} {";
		for (auto& el : m_node->lastpos) {
			std::cout << el << " ";
		}
		std::cout << "}";
		std::cout << std::endl;
		printNFLTree(m_node->right, level + 1);
	}
}

void SynTree::setNFL()
{
	setNFLTree(root);
}

void SynTree::setNFLTree(std::shared_ptr<Node> m_node)
{
	static size_t num_symbol = 1;
	if (m_node)
	{
		setNFLTree(m_node->left);
		setNFLTree(m_node->right);
		if (m_node->c == '^' && !m_node->escaping)
			m_node->nullable = true;
		else if (m_node->c == '.' && !m_node->escaping) {
			m_node->nullable = m_node->left->nullable && m_node->right->nullable;

			if (m_node->left->nullable) {
				m_node->firstpos.insert(m_node->left->firstpos.begin(), m_node->left->firstpos.end());
				m_node->firstpos.insert(m_node->right->firstpos.begin(), m_node->right->firstpos.end());
			}
			else
				m_node->firstpos.insert(m_node->left->firstpos.begin(), m_node->left->firstpos.end());

			if (m_node->right->nullable) {
				m_node->lastpos.insert(m_node->left->lastpos.begin(), m_node->left->lastpos.end());
				m_node->lastpos.insert(m_node->right->lastpos.begin(), m_node->right->lastpos.end());
			}
			else
				m_node->lastpos.insert(m_node->right->lastpos.begin(), m_node->right->lastpos.end());
		}
		else if (m_node->c == '|' && !m_node->escaping) {
			m_node->nullable = m_node->left->nullable || m_node->right->nullable;

			m_node->firstpos.insert(m_node->left->firstpos.begin(), m_node->left->firstpos.end());
			m_node->firstpos.insert(m_node->right->firstpos.begin(), m_node->right->firstpos.end());

			m_node->lastpos.insert(m_node->left->lastpos.begin(), m_node->left->lastpos.end());
			m_node->lastpos.insert(m_node->right->lastpos.begin(), m_node->right->lastpos.end());

		}
		else if (m_node->c == '+' && !m_node->escaping) {
			m_node->nullable = false;
			m_node->firstpos.insert(m_node->left->firstpos.begin(), m_node->left->firstpos.end());
			m_node->lastpos.insert(m_node->left->lastpos.begin(), m_node->left->lastpos.end());
		}
		else {
			m_node->nullable = false;
			m_node->firstpos.insert(num_symbol);
			m_node->lastpos.insert(num_symbol);
			std::set<size_t> set = {};
			followpos.emplace(std::make_pair(num_symbol, m_node->c), set);
			num_symbol++;
		}

		setFPTree(m_node);
	}
}

void SynTree::setFPTree(std::shared_ptr<Node> m_node)
{
	if (m_node->c == '.' && !m_node->escaping) {
		for (auto& num_left : m_node->left->lastpos) {
			for (auto& [key, value] : followpos) {
				if (key.first == num_left) {
					for (auto& num_right : m_node->right->firstpos) {
						value.insert(num_right);
					}
				}
			}
		}
	}
	else if (m_node->c == '+' && !m_node->escaping) {
		for (auto& num_left : m_node->left->lastpos) {
			for (auto& [key, value] : followpos) {
				if (key.first == num_left) {
					for (auto& num_right : m_node->left->firstpos) {
						value.insert(num_right);
					}
				}
			}
		}
	}
}

//void SynTree::copyNode(size_t idx_c, size_t idx_p)
//{
//	nodes[idx_p]->str = nodes[idx_c]->str;
//	nodes[idx_p]->escaping = nodes[idx_c]->escaping;
//  nodes[idx_p]->is_root_subtree = nodes[idx_c]->is_root_subtree;
//	nodes[idx_p]->parent = nodes[idx_c]->parent;
//	nodes[idx_p]->left = nodes[idx_c]->left;
//	nodes[idx_p]->right = nodes[idx_c]->right;
//}