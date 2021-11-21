#include "synTree.hpp"

void SynTree::process(std::string str)
{
	regular = str;
	process();
}

void SynTree::process()
{
	if (regular == "") {
		throw "Error! Empty Regular expression!";
		return;
	}
	
	regular.insert(0, "((");
	regular += ")&)";
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
					throw "Error! Invalid '(' or ')'";
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
		throw "Error! Invalid '(' or ')'";
		return;
	}

	root = nodes[0];
}

void SynTree::bracketHandler(size_t first, size_t last)
{
	if (last - first == 1) {
		throw "Error! Invalid ()";
		return;
	}
	createNodes(first, last);
	getFLErrors();

	nodes.erase(std::remove_if(nodes.begin(), nodes.end(), [](const std::shared_ptr<Node>& node) { return (node->c == '#' && node->is_escaping == false); }), nodes.end());  // delete all not is_escaping '#'

	std::string str_capture;
	bool capture_flag = false;
	captureHandler(str_capture, capture_flag, first);

	rangeHandler();
	concatinationHandler();
	orHandler();

	if (capture_flag) {
		for (auto& [key, value] : capture) {
			if (key == std::stoi(str_capture)) {
				throw "Error! Number of capture repeated more than once";
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
	std::vector<std::pair<size_t, bool>> captures_and_conc;
	bool true_capture_flag = true;
	size_t count_capture = 0;
	for (size_t i = num_of_brackets_in_nodes; i < nodes.size(); i++) {  // search "n:"
		if (nodes[i]->c == ':' && !nodes[i]->is_escaping && capture_flag) {  // invalid ":"
			throw "Error! Invalid ':'";
			return;
		}

		if (nodes[i]->c == ':' && !nodes[i]->is_escaping) {
			if (!true_capture_flag || str_capture == "") {
				throw "Error! Invalid capture n:";
				return;
			}
			capture_flag = true;
		}

		if ((nodes[i]->c > '9' || nodes[i]->c < '0') && !capture_flag)
			true_capture_flag = false;

		if (nodes[i]->c == '(' && !nodes[i]->is_escaping && !nodes[i]->is_root_subtree) {
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
		else {
			nodes[i]->num_capture = std::stoi(str_capture);
			nodes[i]->capture_mode = 1;
			if (nodes[i]->is_root_subtree) {
				isExistCapture(nodes[i], std::stoi(str_capture));
				getModeTwoLower(nodes[i], std::stoi(str_capture), 1);
			}
		}

		for (auto& [key, value] : capture) {
			if (value == nodes[i]) {
				captures_and_conc.push_back(std::make_pair(key, false));
			}
		}

		if (nodes[i]->c == '|' && !nodes[i]->is_escaping && !nodes[i]->is_root_subtree) {
			for (size_t j = 0; j < captures_and_conc.size(); j++) {
				captures_and_conc[j].second = true;
			}
		}

		if (nodes[i]->c == '\\' && !nodes[i]->is_escaping) { // search "\n"
			size_t j = i + 1, count = 0;
			std::string str_num_capture = {};
			while (j < nodes.size() && nodes[j]->c > '0' && nodes[j]->c < '9') {
				str_num_capture += nodes[j]->c;
				j++;
				count++;
			}
			if (j == i + 1) {
				throw "Error! Invalid capture \\n";
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
				throw "Error! Number of capture doesn't exist";
				return;
			}
			for (size_t j = 0; j < captures_and_conc.size(); j++) {
				if (std::stoi(str_num_capture) == captures_and_conc[j].first && captures_and_conc[j].second) {
					throw "Error! '|' between n: and \\n with equals n";
				}
			}
			nodes[i]->capture_repeat = std::stoi(str_num_capture);
			nodes[i]->capture_mode = 2;
			getModeTwoLower(nodes[i], std::stoi(str_num_capture), 2);
			nodes.erase(nodes.begin() + i + 1, nodes.begin() + i + count + 1);
		}

		if (i == nodes.size() - 1 && !capture_flag) {
			true_capture_flag = false;
		}

		if (i == nodes.size() - 1 && true_capture_flag)
			nodes.erase(nodes.begin() + num_of_brackets_in_nodes, nodes.begin() + num_of_brackets_in_nodes + count_capture + 1);
	}
}

void SynTree::getModeTwoLower(std::shared_ptr<Node> m_node, int num_capture, int capture_mode)
{
	if (!m_node) {
		return;
	}
	getModeTwoLower(m_node->left, num_capture, capture_mode);
	getModeTwoLower(m_node->right, num_capture, capture_mode);

	m_node->capture_repeat = num_capture;
	m_node->capture_mode = capture_mode;
	m_node->num_capture = num_capture;
}

void SynTree::isExistCapture(std::shared_ptr<Node> m_node, int num_capture)
{
	if (!m_node) {
		return;
	}
	isExistCapture(m_node->left, num_capture);
	isExistCapture(m_node->right, num_capture);

	if (m_node->num_capture != -1 && m_node->num_capture != num_capture) {
		throw "Error! One capture in another capture";
	}
}

void SynTree::rangeHandler()
{
	for (size_t i = num_of_brackets_in_nodes; i < nodes.size(); i++) {
		if (nodes[i]->c == '}' && !nodes[i]->is_escaping) {
			throw "Error! Can't find '{'";
			return;
		}
		if (nodes[i]->c == '{' && !nodes[i]->is_escaping) {  // search '{'
			if (i == num_of_brackets_in_nodes) {
				throw "Error! Nothing before '{'";
				return;
			}
			if (i > num_of_brackets_in_nodes && checkMeta(nodes[i - 1]->c, 0) && !nodes[i - 1]->is_escaping && !nodes[i - 1]->is_root_subtree) {
				throw "Error! Invalid '|' or '.' before '{'";
				return;
			}
			nodes.erase(nodes.begin() + i);
			std::string str = "";
			size_t j = i, count = 0, x = 0, y = 0;
			bool coma_flag = false;
			while (true) {
				if (j == nodes.size() - 1 && !(nodes[j]->c == '}' && !nodes[j]->is_escaping && !nodes[j]->is_root_subtree)) {
					throw "Error! Can't find '}'";
					return;
				}
				count++;
				if (nodes[j]->c == ',' && !nodes[j]->is_escaping && !nodes[j]->is_root_subtree) {
					coma_flag = true;
					if (nodes[j + 1]->c == '}') {
						count++;
						nodes.erase(nodes.begin() + i, nodes.begin() + i + count);
						x = std::stoi(str);
						oneNumRangeHandler(x, i);
						i--;
						break;
					}
					else {
						x = std::stoi(str);
						str = "";
						j++;
						continue;
					}
				}

				if (nodes[j]->c == '}' && !nodes[j]->is_escaping && !nodes[j]->is_root_subtree) {
					if (!coma_flag || x > std::stoi(str)) {
						throw "Error! Invalid range {}";
						return;
					}
					y = std::stoi(str);
					nodes.erase(nodes.begin() + i, nodes.begin() + i + count);
					twoNumRangeHandler(x, y, i);
					i--;
					break;
				}

				if (nodes[j]->c > '9' || nodes[j]->c < '0') {
					throw "Error! Invalid range {}";
					return;
				}

				str += nodes[j]->c;
				j++;
			}
		}

		if (nodes[i]->c == '+' && !nodes[i]->is_escaping && !nodes[i]->is_root_subtree) {  // search '+'
			if (i > num_of_brackets_in_nodes && checkMeta(nodes[i - 1]->c, 0) && !nodes[i - 1]->is_escaping && !nodes[i - 1]->is_root_subtree) {
				throw "Error! Invalid '|' or '.' before '+'";
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

	bool is_null_x = false;
	if (!x) {
		x++;
		is_null_x = true;
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
	if (is_null_x) {
		nodes.insert(nodes.begin() + i, std::make_shared<Node>('^', false, true));
		nodes.insert(nodes.begin() + i, std::make_shared<Node>('|', false, true));
		nodes[i]->left = nodes[i - 1];
		nodes[i]->right = nodes[i + 1];
		nodes[i]->is_root_subtree = true;
		nodes[i - 1]->parent = nodes[i];
		nodes[i + 1]->parent = nodes[i];
		nodes.erase(nodes.begin() + i + 1);
		nodes.erase(nodes.begin() + i - 1);
	}
}

void SynTree::concatinationHandler()
{
	for (size_t i = num_of_brackets_in_nodes + 1; i < nodes.size(); i++) {  // search concatination
		getErrors(i);
		if (nodes[i]->c == '.' && !nodes[i]->is_escaping && !nodes[i]->is_root_subtree) {
			nodes[i]->left = nodes[i - 1];
			nodes[i]->right = nodes[i + 1];
			nodes[i]->is_root_subtree = true;
			nodes[i - 1]->parent = nodes[i];
			nodes[i + 1]->parent = nodes[i];
			nodes.erase(nodes.begin() + i - 1);
			nodes.erase(nodes.begin() + i);
			i--;
		}
		else if ((nodes[i]->c != '|') || (nodes[i]->c == '|' && nodes[i]->is_escaping) || (nodes[i]->c == '|' && nodes[i]->is_root_subtree)) {
			if (nodes[i - 1]->c == '|' && !nodes[i - 1]->is_escaping && !nodes[i - 1]->is_root_subtree)
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
		if (nodes[i]->c == '|' && !nodes[i]->is_escaping && !nodes[i]->is_root_subtree) {
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

	auto new_node = std::make_shared<Node>(tree_c->c, tree_c->is_escaping, tree_c->is_root_subtree);
	new_node->parent = cur_parent;
	new_node->left = copyTree(tree_c->left, new_node);
	new_node->right = copyTree(tree_c->right, new_node);

	return new_node;
}

void SynTree::getErrors(const size_t idx)
{
	if (idx < nodes.size() - 1 && checkMeta(nodes[idx + 1]->c, 0) && !nodes[idx + 1]->is_escaping && !nodes[idx + 1]->is_root_subtree && checkMeta(nodes[idx]->c, 0) && !nodes[idx]->is_escaping && !nodes[idx]->is_root_subtree) {
		throw "Error! Invalid '|' or '.' or '+'";
		return;
	}
}

void SynTree::getFLErrors()
{
	if (checkMeta(nodes[num_of_brackets_in_nodes]->c, 0) && !nodes[num_of_brackets_in_nodes]->is_escaping && !nodes[num_of_brackets_in_nodes]->is_root_subtree ||
		checkMeta(nodes[nodes.size() - 1]->c, 1) && !nodes[nodes.size() - 1]->is_escaping && !nodes[nodes.size() - 1]->is_root_subtree) {
		throw "Error! Invalid '|' or '.' or '+'";
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
		std::cout << key.first[0] << " " << key.second << " ";
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
		std::cout << m_node->is_nullable << " {";
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
	setNFLTree(root, false);
	setNFLTree(nullptr, true);
}

void SynTree::setNFLTree(std::shared_ptr<Node> m_node, bool null_flag)
{
	static int num_symbol = 1;
	if (m_node)
	{
		setNFLTree(m_node->left, false);
		setNFLTree(m_node->right, false);
		if (m_node->c == '^' && !m_node->is_escaping)
			m_node->is_nullable = true;
		else if (m_node->c == '.' && !m_node->is_escaping) {
			m_node->is_nullable = m_node->left->is_nullable && m_node->right->is_nullable;

			if (m_node->left->is_nullable) {
				m_node->firstpos.insert(m_node->left->firstpos.begin(), m_node->left->firstpos.end());
				m_node->firstpos.insert(m_node->right->firstpos.begin(), m_node->right->firstpos.end());
			}
			else
				m_node->firstpos.insert(m_node->left->firstpos.begin(), m_node->left->firstpos.end());

			if (m_node->right->is_nullable) {
				m_node->lastpos.insert(m_node->left->lastpos.begin(), m_node->left->lastpos.end());
				m_node->lastpos.insert(m_node->right->lastpos.begin(), m_node->right->lastpos.end());
			}
			else
				m_node->lastpos.insert(m_node->right->lastpos.begin(), m_node->right->lastpos.end());
		}
		else if (m_node->c == '|' && !m_node->is_escaping) {
			m_node->is_nullable = m_node->left->is_nullable || m_node->right->is_nullable;

			m_node->firstpos.insert(m_node->left->firstpos.begin(), m_node->left->firstpos.end());
			m_node->firstpos.insert(m_node->right->firstpos.begin(), m_node->right->firstpos.end());

			m_node->lastpos.insert(m_node->left->lastpos.begin(), m_node->left->lastpos.end());
			m_node->lastpos.insert(m_node->right->lastpos.begin(), m_node->right->lastpos.end());

		}
		else if (m_node->c == '+' && !m_node->is_escaping) {
			m_node->is_nullable = false;
			m_node->firstpos.insert(m_node->left->firstpos.begin(), m_node->left->firstpos.end());
			m_node->lastpos.insert(m_node->left->lastpos.begin(), m_node->left->lastpos.end());
		}
		else {
			m_node->is_nullable = false;
			m_node->firstpos.insert(num_symbol);
			m_node->lastpos.insert(num_symbol);
			std::set<size_t> set = {};
			std::vector<int> vec = { num_symbol, m_node->capture_mode, m_node->capture_repeat, m_node->num_capture };
			followpos.emplace(std::make_pair(vec, m_node->c), set);
			num_symbol++;
		}

		setFPTree(m_node);
	}

	if (null_flag)
		num_symbol = 1;
}

void SynTree::setFPTree(std::shared_ptr<Node> m_node)
{
	if (m_node->c == '.' && !m_node->is_escaping) {
		for (auto& num_left : m_node->left->lastpos) {
			for (auto& [key, value] : followpos) {
				if (key.first[0] == num_left) {
					for (auto& num_right : m_node->right->firstpos) {
						value.insert(num_right);
					}
				}
			}
		}
	}
	else if (m_node->c == '+' && !m_node->is_escaping) {
		for (auto& num_left : m_node->left->lastpos) {
			for (auto& [key, value] : followpos) {
				if (key.first[0] == num_left) {
					for (auto& num_right : m_node->left->firstpos) {
						value.insert(num_right);
					}
				}
			}
		}
	}
}

void SynTree::startInversion()
{
	inversion(root);
}

void SynTree::inversion(std::shared_ptr<Node> m_node)
{
	if (m_node)
	{
		inversion(m_node->left);
		inversion(m_node->right);
		if (m_node->c == '.' && m_node != root) {
			std::swap(m_node->left, m_node->right);
		}
	}
}