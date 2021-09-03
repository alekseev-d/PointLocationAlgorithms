#pragma once
#include "cells.h"
#include "graph.h"
#include <algorithm>
#include <vector>

typedef std::set<std::shared_ptr<cells> > cell_set;
typedef std::map<node, cell_set> cell_map;
typedef std::vector<node> node_vec;
typedef std::vector<edge> edge_vec;

const int MAX_DEGREE = 8;

node_vec find_outer_triangle(const node_vec& p_points)
{
	node_vec res;
	node lower_left;
	double c = 0;
	for (const auto& iter : p_points)
	{
		if (iter.m_x < lower_left.m_x)
		{
			lower_left.m_x = iter.m_x;
		}
		if (iter.m_y < lower_left.m_y)
		{
			lower_left.m_y = iter.m_y;
		}
		if (iter.m_x + iter.m_y > c)
		{
			c = iter.m_x + iter.m_y;
		}
	}
	lower_left += node(-10, -10);
	c += 10;
	res.push_back(lower_left);
	res.push_back(node(c - lower_left.m_y, lower_left.m_y));
	res.push_back(node(lower_left.m_x, c - lower_left.m_x));
	return res;
}

void add_triangle(graph& p_graph, const node& p_node1, const node& p_node2, const node& p_node3, bool p_is_inside, cell_map& p_triangles, cell_set& p_generated_triangles)
{
	p_graph.add_edge(p_node1, p_node2);
	p_graph.add_edge(p_node2, p_node3);
	p_graph.add_edge(p_node3, p_node1);
	node_vec vec = { p_node1,p_node2,p_node3 };
	auto cell = std::make_shared<cells>(vec, p_is_inside);
	p_triangles[p_node1].insert(cell);
	p_triangles[p_node2].insert(cell);
	p_triangles[p_node3].insert(cell);
	p_generated_triangles.insert(cell);
}

void add_cell(const cell& p_cell, cell_map& p_map, const node_vec& p_vec)
{
	node_vec vec;
	vec.reserve(p_cell.m_nodes.size());
	for (const auto& iter : p_cell.m_nodes)
	{
		vec.push_back(p_vec[iter]);
	}
	auto cell = std::make_shared<cells>(vec, false);
	for (const auto& iter : vec)
	{
		p_map[iter].insert(cell);
	}
}

void triangulate_polygon(const node_vec& p_points, graph& p_graph, cell_map& p_triangles, bool p_is_inside, cell_set& p_generated_triangles)
{
	node_vec avail_points;
	for (const auto& iter : p_points)
	{
		while (avail_points.size() > 1)
		{
			auto jt = avail_points.rbegin();
			if (!is_ear(*(jt + 1), *jt, iter, p_points))
			{
				break;
			}
			add_triangle(p_graph, *(jt + 1), *jt, iter, p_is_inside, p_triangles, p_generated_triangles);
			avail_points.pop_back();
		}
		avail_points.push_back(iter);
	}
}

void triangulate_pockets(const node_vec& p_points, graph& p_graph, node_vec& p_convex_hull, cell_map& p_triangles)
{
	cell_set tmp;
	int leftmost = 0;
	for (int i = 0; i < p_points.size(); ++i)
	{
		if (p_points[i].m_x < p_points[leftmost].m_x)
		{
			leftmost = i;
		}
	}
	int i = leftmost;
	p_convex_hull.push_back(p_points[(i++) % p_points.size()]);
	p_convex_hull.push_back(p_points[(i++) % p_points.size()]);
	for (; i - leftmost != p_points.size() + 1; ++i)
	{
		auto node = p_points[i % p_points.size()];
		while (p_convex_hull.size() > 1)
		{
			auto jt = p_convex_hull.rbegin();
			if (!is_right_turn(*(jt + 1), *jt, node))
			{
				break;
			}
			bool res = true;
			for (const auto& iter : p_points)
			{
				if (iter == *(jt + 1) || iter == *jt || iter == node)
				{
					continue;
				}
				if (inside_triangle(node_vec({ node, *jt, *(jt + 1) }), iter))
				{
					res = false;
					break;
				}
			}
			if (!res)
			{
				break;
			}
			add_triangle(p_graph, node, *jt, *(jt + 1), false, p_triangles, tmp);
			p_convex_hull.pop_back();
		}
		p_convex_hull.push_back(node);
	}
}

void triangulate_with_outer_triangle(const node_vec& p_convex_hull, const node_vec& p_outer_points, graph& p_graph, cell_map& p_triangles)
{
	cell_set tmp;
	add_triangle(p_graph, p_convex_hull[0], p_outer_points[2], p_outer_points[0], false, p_triangles, tmp);
	int last_seen = 0;
	for (int i = 1; i < p_convex_hull.size(); ++i)
	{
		if (is_left_turn(p_outer_points[last_seen], p_convex_hull[i], p_convex_hull[i - 1]))
		{
			add_triangle(p_graph, p_convex_hull[i - 1], p_outer_points[last_seen], p_convex_hull[i], false, p_triangles, tmp);
		}
		if (last_seen == 2)
		{
			continue;
		}
		if (is_right_turn(p_outer_points[last_seen + 1], p_convex_hull[i], p_convex_hull[i + 1]))
		{
			add_triangle(p_graph, p_outer_points[last_seen], p_outer_points[last_seen + 1], p_convex_hull[i], false, p_triangles, tmp);
			last_seen += 1;
		}
	}
}

void initial_triangulation(const node_vec& p_points, const node_vec& p_outer_points, graph& p_graph, cell_map& p_triangles, const cell_map& p_cells, const node_vec& p_vec)
{
	cell_set tris;
	triangulate_polygon(p_points, p_graph, p_triangles, true, tris);
	node_vec convex_hull;
	triangulate_pockets(p_points, p_graph, convex_hull, p_triangles);
	triangulate_with_outer_triangle(convex_hull, p_outer_points, p_graph, p_triangles);
	for (const auto& it : p_triangles)
	{
		for (const auto& it2 : it.second)
		{
			for (const auto& iter : p_cells)
			{
				for (const auto& iter2 : iter.second)
				{
					if (intersects(*it2, *iter2))
					{
						it2->add_child(iter2);
					}
				}
			}
		}
	}
}

cell_set reform(cell_set const& p_cells)
{
	cell_set ret;
	for (const auto& iter : p_cells)
	{
		std::shared_ptr<cells>* triter = new std::shared_ptr<cells>(iter);
		ret.insert(*triter);
	}
	return ret;
}

void retriangulate(node_vec& p_poly, const node& p_node, graph& p_graph, cell_map& p_triangles)
{
	const cell_set& old_triangles = reform(p_triangles[p_node]);
	cell_set new_triangles;
	triangulate_polygon(p_poly, p_graph, p_triangles, false, new_triangles);
	for (const auto& iter1 : old_triangles)
	{
		for (const auto& iter2 : new_triangles)
		{
			if (intersects(*iter1, *iter2))
			{
				iter2->add_child(iter1);
			}
		}
		for (auto& el : p_triangles)
		{
			int res = el.second.erase(iter1);
		}
	}
	p_triangles.erase(p_node);
}

bool refine(graph& p_graph, cell_map& p_triangles)
{
	node_vec iset = p_graph.independent_set(MAX_DEGREE);
	if (iset.empty())
	{
		return false;
	}
	for (const auto& iter : iset)
	{
		node_vec poly = sort_counter_clockwise(iter, p_graph.neighbours(iter));
		retriangulate(poly, iter, p_graph, p_triangles);
	}
	p_graph.remove(iset);
	return true;
}

std::shared_ptr<cells> refinement(graph& p_graph, cell_map& p_triangles)
{
	for (;;)
	{
		if (!refine(p_graph, p_triangles))
			break;
	}
	return *(p_triangles.begin()->second.begin());
}

bool sort_pred(const node& p_node1, const node& p_node2)
{
	return p_node1.m_x < p_node2.m_x || p_node1.m_x == p_node2.m_x && p_node1.m_y < p_node2.m_y;
}

bool up_func(const node& p_node1, const node& p_node2, const node& p_node3)
{
	return p_node1.m_x * (p_node2.m_y - p_node3.m_y) + p_node2.m_x * (p_node3.m_y - p_node1.m_y) + p_node3.m_x * (p_node1.m_y - p_node2.m_y) < 0;
}

bool down_func(const node& p_node1, const node& p_node2, const node& p_node3)
{
	return p_node1.m_x * (p_node2.m_y - p_node3.m_y) + p_node2.m_x * (p_node3.m_y - p_node1.m_y) + p_node3.m_x * (p_node1.m_y - p_node2.m_y) > 0;
}

node_vec convex_hull(const node_vec& p_vec)
{
	node_vec res(p_vec);
	if (res.size() == 1)
	{
		return res;
	}
	std::sort(res.begin(), res.end(), &sort_pred);
	node p_node1 = res[0], p_node2 = res.back();
	node_vec up, down;
	up.push_back(p_node1);
	down.push_back(p_node1);
	for (int i = 1; i < res.size(); ++i)
	{
		if (i == res.size() - 1 || up_func(p_node1, res[i], p_node2))
		{
			while (up.size() >= 2 && !up_func(up[up.size() - 2], up[up.size() - 1], res[i]))
			{
				up.pop_back();
			}
			up.push_back(res[i]);
		}
		if (i == res.size() - 1 || down_func(p_node1, res[i], p_node2))
		{
			while (down.size() >= 2 && !down_func(down[down.size() - 2], down[down.size() - 1], res[i]))
			{
				down.pop_back();
			}
			down.push_back(res[i]);
		}
	}
	res.clear();
	for (int i = 0; i < up.size(); ++i)
	{
		res.push_back(up[i]);
	}
	for (int i = down.size() - 2; i > 0; --i)
	{
		res.push_back(down[i]);
	}
	return res;
}