#include "cells.h"

bool intersects(const cells& p_cell1, const cells& p_cell2)
{
	edge_vec cell1, cell2;
	for (int i = 0; i < p_cell1.m_cells_nodes.size(); ++i)
	{
		if (i == p_cell1.m_cells_nodes.size() - 1)
		{
			cell1.push_back(edge(p_cell1.m_cells_nodes[i], p_cell1.m_cells_nodes[0]));
		}
		else
		{
			cell1.push_back(edge(p_cell1.m_cells_nodes[i], p_cell1.m_cells_nodes[i + 1]));
		}
	}
	for (int i = 0; i < p_cell2.m_cells_nodes.size(); ++i)
	{
		if (i == p_cell2.m_cells_nodes.size() - 1)
		{
			cell2.push_back(edge(p_cell2.m_cells_nodes[i], p_cell2.m_cells_nodes[0]));
		}
		else
		{
			cell2.push_back(edge(p_cell2.m_cells_nodes[i], p_cell2.m_cells_nodes[i + 1]));
		}
	}
	bool res = false;
	for (const auto& iter1 : cell1)
	{
		for (const auto& iter2 : cell2)
		{
			if (intersects(iter1, iter2))
				res = true;
		}
	}
	return res;
}

bool inside_cells(const node_vec& p_vec, const node& p_node)
{
	double a, b, a1, a2, b1, b2, grad = 0;
	for (int i = 0; i < p_vec.size(); i++)
	{
		if (i < p_vec.size() - 1)
		{
			a1 = p_vec[i].m_x - p_node.m_x;
			a2 = p_vec[i].m_y - p_node.m_y;
			b1 = p_vec[i + 1].m_x - p_node.m_x;
			b2 = p_vec[i + 1].m_y - p_node.m_y;
			a = sqrt(a1 * a1 + a2 * a2);
			b = sqrt(b1 * b1 + b2 * b2);
			grad += acos((a1 * b1 + a2 * b2) / (a * b));
		}
		else
		{
			a1 = p_vec[i].m_x - p_node.m_x;
			a2 = p_vec[i].m_y - p_node.m_y;
			b1 = p_vec[i - (p_vec.size() - 1)].m_x - p_node.m_x;
			b2 = p_vec[i - (p_vec.size() - 1)].m_y - p_node.m_y;
			a = sqrt(a1 * a1 + a2 * a2);
			b = sqrt(b1 * b1 + b2 * b2);
			grad += acos((a1 * b1 + a2 * b2) / (a * b));
		}
	}
	if (grad >= 4 * asin(1))
	{
		return true;
	}
	return false;
}

cells::cells()
{
	sCount++;
	m_id = sCount;
	m_is_inside = false;
}

cells::cells(const node_vec& p_cells_nodes, bool p_is_inside)
{
	sCount++;
	m_id = sCount;
	m_cells_nodes = p_cells_nodes;
	m_is_inside = p_is_inside;
}

cells::~cells()
{
	--sCount;
}

bool cells::inside(node const& p_node) const
{
	if (m_cells_nodes.size() == 3)
	{
		return inside_triangle(m_cells_nodes, p_node);
	}
	else
	{
		return inside_cells(m_cells_nodes, p_node);
	}
}

bool cells::query(node const& p_node, std::vector<int>& p_vec, bool& find)
{
	if (!inside(p_node))
	{
		return false;
	}
	if (m_children.empty() && find == 0)
	{
		p_vec.push_back(m_id);
		find = 1;
		return m_is_inside;
	}
	for (auto t : m_children)
	{
		if (t->query(p_node, p_vec, find))
		{
			return true;
		}
	}
	return false;
}

void cells::add_child(cell_ptr const& p_child)
{
	m_children.insert(p_child);
}

bool cells::adding(const cell_ptr& p_cell)
{
	if (!intersects(*this,*p_cell))
	{
		return false;
	}
	if (m_children.empty())
	{
		return m_is_inside;
	}
	for (auto t : m_children)
	{
		if (t->adding(p_cell))
		{
			t->add_child(p_cell);
			return true;
		}
	}
	return false;
}

