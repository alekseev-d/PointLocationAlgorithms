#include "graph.h"

graph::graph(const node_vec& p_special_points) : m_special_points(p_special_points)
{
	add_poly(p_special_points);
}

void graph::add(const node& p_node)
{
	m_graph[p_node] = std::set<node>();
}

void graph::add_edge(const node& p_node1, const node& p_node2)
{
	m_graph[p_node1].insert(p_node2);
	m_graph[p_node2].insert(p_node1);
}

void graph::add_poly(const node_vec& p_points)
{
	auto fst_point = p_points.begin();
	add(*fst_point);
	auto prev = fst_point;
	for (auto it = fst_point + 1; it != p_points.end(); ++it, ++prev)
	{
		add(*it);
		add_edge(*prev, *it);
	}
	add_edge(*prev, *fst_point);
}

edge_vec graph::edges() const
{
	edge_vec res;
	for (auto el : m_graph)
	{
		auto p1 = el.first;
		for (auto p2 : el.second)
		{
			if (p2 < p1)
			{
				continue;
			}
			res.push_back(edge(p1, p2));
		}
	}
	return res;
}

node_vec graph::independent_set(int p_max_degree) const
{
	node_vec res;
	std::set<node> masked;
	for (auto pt : m_special_points)
	{
		masked.insert(pt);
	}
	for (auto el : m_graph)
	{
		if (el.second.size() > p_max_degree)
		{
			continue;
		}
		if (masked.find(el.first) != masked.end())
		{
			continue;
		}
		masked.insert(el.second.begin(), el.second.end());
		res.push_back(el.first);
	}
	return res;
}

std::set<node> graph::neighbours(const node& p_node) const
{
	return m_graph.at(p_node);
}

void graph::remove(const node& p_node)
{
	for (auto el : m_graph[p_node])
	{
		m_graph[el].erase(p_node);
	}
	m_graph.erase(p_node);
}

void graph::remove(const node_vec& p_vec)
{
	for (auto pt : p_vec)
	{
		remove(pt);
	}
}
