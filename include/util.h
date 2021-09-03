#pragma once
#include <vector>
#include "node.h"
#include <algorithm>
#include <cmath>

typedef std::vector<node> node_vec;
typedef std::vector<edge> edge_vec;

inline double determinant(const node& p_node1, const node& p_node2, const node& p_node3)
{
	return (p_node2.m_x * p_node3.m_y - p_node3.m_x * p_node2.m_y) - (p_node1.m_x * p_node3.m_y - p_node3.m_x * p_node1.m_y) + (p_node1.m_x * p_node2.m_y - p_node2.m_x * p_node1.m_y);
}

template<class T>
int sign(T t)
{
	if (t < 0)
	{
		return -1;
	}
	if (t == 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

inline bool is_right_turn(const node& p_node1, const node& p_node2, const node& p_node3)
{
	return determinant(p_node1, p_node2, p_node3) < 0;
}

inline bool is_left_turn(const node& p_node1, const node& p_node2, const node& p_node3)
{
	return determinant(p_node1, p_node2, p_node3) > 0;
}

inline bool intersects(const edge& p_edge1, const edge& p_edge2)
{
	int r1 = sign(determinant(p_edge1[0], p_edge1[1], p_edge2[0]));
	int r2 = sign(determinant(p_edge1[0], p_edge1[1], p_edge2[1]));
	int r3 = sign(determinant(p_edge2[0], p_edge2[1], p_edge1[0]));
	int r4 = sign(determinant(p_edge2[0], p_edge2[1], p_edge1[1]));
	return (r1 * r2 <= 0) && (r3 * r4 <= 0);
}

inline bool is_counter_clockwise(const node_vec& p_points)
{
	int leftmost = 0;
	for (int i = 0; i != p_points.size(); ++i)
	{
		if (p_points[i].m_x < p_points[leftmost].m_x)
		{
			leftmost = i;
		}
	}
	int next = (leftmost + 1) % p_points.size();
	int prev = (p_points.size() + leftmost - 1) % p_points.size();
	return p_points[prev].m_y > p_points[next].m_y;
}

template<class Cont>
node_vec sort_counter_clockwise(const node& p_node, const Cont& p_points)
{
	node_vec res(p_points.size());
	std::partial_sort_copy(p_points.begin(), p_points.end(), res.begin(), res.end(), [&p_node](const node& p_node1, const node& p_node2)
		{
			double a1 = std::atan2(p_node1.m_y - p_node.m_y, p_node1.m_x - p_node.m_x);
			double a2 = std::atan2(p_node2.m_y - p_node.m_y, p_node2.m_x - p_node.m_x);
			return a1 < a2;
		});
	return res;
}

inline bool inside_triangle(const node_vec& p_vec, const node& p_node)
{
	int r1 = sign(determinant(p_node, p_vec[1], p_vec[0]));
	int r2 = sign(determinant(p_node, p_vec[2], p_vec[1]));
	int r3 = sign(determinant(p_node, p_vec[0], p_vec[2]));
	return (r1 <= 0 && r2 <= 0 && r3 <= 0);
}

inline bool is_ear(const node& p_node1, const node& p_node2, const node& p_node3, const node_vec& p_points)
{
	if (!is_left_turn(p_node1, p_node2, p_node3))
	{
		return false;
	}
	for (auto pt : p_points)
	{
		if (pt == p_node1 || pt == p_node2 || pt == p_node3)
		{
			continue;
		}
		if (inside_triangle(node_vec({ p_node1, p_node2, p_node3 }), pt))
		{
			return false;
		}
	}
	return true;
}
