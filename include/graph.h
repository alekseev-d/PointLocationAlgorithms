#pragma once
#include <map>
#include <set>
#include "node.h"
#include "util.h"

class graph
{
	std::map<node, std::set<node>> m_graph;
	node_vec m_special_points;
public:
	graph() = default;
	graph(const node_vec& p_special_points);
	void add(const node& p_node);
	void add_edge(const node& p_node1, const node& p_node2);
	void add_poly(const node_vec& p_points);
	edge_vec edges() const;
	node_vec independent_set(int p_max_degree) const;
	std::set<node> neighbours(const node& p_node) const;
	void remove(const node& p_node);
	void remove(const node_vec& p_vec);
};
