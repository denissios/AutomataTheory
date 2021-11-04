#ifndef __SYNTREE_HPP_INCLUDED__
#define __SYNTREE_HPP_INCLUDED__

#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <memory>

class SynTree
{
public:
	SynTree(std::string regular) : regular{ regular }, root{}, num_of_brackets_in_nodes{ 0 }, capture{}, followpos{}, nodes{}, left_brackets{}, right_brackets{} {}
	void process();
	void setNFL();
	void print();

private:
	struct Node
	{
		Node(const char c, bool escaping, bool is_root_subtree = false) : c{ c }, escaping{ escaping }, is_root_subtree{ is_root_subtree }, parent{}, left{}, right{}, nullable{}, firstpos{}, lastpos{} {}

		const char c;
		bool escaping;
		bool is_root_subtree;
		std::shared_ptr<Node> parent;
		std::shared_ptr<Node> left;
		std::shared_ptr<Node> right;
		bool nullable;
		std::set<size_t> firstpos;
		std::set<size_t> lastpos;
	};

	void createNodes(size_t first, size_t last);
	void bracketHandler(size_t first, size_t last);
	void captureHandler(std::string& str_capture, bool& capture_flag, size_t& first);
	void rangeHandler();
	void oneNumRangeHandler(size_t& x, size_t& i);
	void twoNumRangeHandler(size_t& x, size_t& y, size_t& i);
	void concatinationHandler();
	void orHandler();

	std::shared_ptr<Node> copyTree(std::shared_ptr<Node> tree_c, std::shared_ptr<Node> cur_parent);
	void getErrors(const size_t idx);
	void getFLErrors();
	bool checkMeta(const char elem, const size_t end);
	void printTree(std::shared_ptr<Node> m_node, size_t level);
	void printNFLTree(std::shared_ptr<Node> m_node, size_t level);
	void setNFLTree(std::shared_ptr<Node> m_node);
	void setFPTree(std::shared_ptr<Node> m_node);
	//void copyNode(size_t idx_c, size_t idx_p);

	const std::array<char, 3> META = { '|', '.', '+' };

	std::string				regular;
	std::shared_ptr<Node>	root;
	size_t					num_of_brackets_in_nodes;

	std::map<size_t, std::shared_ptr<Node>>					capture;
	std::map<std::pair<size_t, char>, std::set<size_t>>		followpos;
	std::vector<std::shared_ptr<Node>>						nodes;
	std::vector<size_t>										left_brackets;
	std::vector<size_t>										right_brackets;

	friend class Graph;
};



#endif //__SYNTREE_HPP_INCLUDED__