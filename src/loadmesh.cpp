#include "loadmesh.h"
#include "kirkpatrick_func.h"
#include <fstream>
#include <iostream>

loadmesh::loadmesh(const std::string& p_str)
{
	std::fstream file;
	file.open(p_str);
	if (!file.is_open())
	{
		throw(std::exception("|  Error: file not found"));
	}
	int size, i = 0, id, c_id = 0;
	file >> size;
	m_nodes.resize(size);
	for (auto& it : m_nodes)
	{
		it.m_id = i;
		++i;
		file >> it.m_x >> it.m_y;
	}
	file >> size;
	m_cells.resize(size);
	for (auto& it : m_cells)
	{
		it.m_cell_id = c_id;
		++c_id;
		file >> it.m_count;
		for (i = 0; i < it.m_count; ++i)
		{
			file >> id;
			it.m_nodes.push_back(id - 1);
		}
	}
	file >> size;
	p_points.resize(size);
	i = 0;
	for (auto& it : p_points)
	{
		it.m_id = i;
		++i;
		file >> it.m_x >> it.m_y;
	}
	node_vec polygon = convex_hull(m_nodes);
	node_vec outer_points = find_outer_triangle(polygon);
	graph graph_nodes = outer_points;
	graph_nodes.add_poly(polygon);
	node_vec points_copy = polygon;
	if (!is_counter_clockwise(polygon))
	{
		std::reverse_copy(polygon.begin(), polygon.end(), points_copy.begin());
	}
	cell_map triangles;
	cell_map old_trian;
	cell_map part_trian;
	node_vec vec;
	for (const auto& iter : m_nodes)
	{
		if (std::find(polygon.begin(), polygon.end(), iter) == polygon.end())
		{
			vec.push_back(iter);
		}
	}
	for (const auto& iter : m_cells)
	{
		add_cell(iter, old_trian, m_nodes);
	}
	for (const auto& iter : old_trian)
	{
		if (std::find(polygon.begin(), polygon.end(), iter.first) == polygon.end())
		{
			part_trian.insert(iter);
		}
	}
	initial_triangulation(points_copy, outer_points, graph_nodes, triangles, part_trian, m_nodes);
	m_top_triangle = refinement(graph_nodes, triangles);
}

void loadmesh::points_location()
{
	std::fstream file;
	file.open("vivod.txt", std::ios::trunc | std::ios::out);
	file << p_points.size() << std::endl;
	if (!file.is_open())
	{
		throw(std::exception("|  Error: file not found"));
	}
	std::vector<int> result;
	result.resize(p_points.size());
	double a, b, a1, a2, b1, b2, grad = 0;
	for (const auto& it : m_cells)
	{
		for (const auto& iter2 : p_points)
		{
			grad = 0;
			for (int i = 0; i < it.m_nodes.size(); i++)
			{
				if (i < it.m_nodes.size() - 1)
				{
					a1 = m_nodes[it.m_nodes[i]].m_x - iter2.m_x;
					a2 = m_nodes[it.m_nodes[i]].m_y - iter2.m_y;
					b1 = m_nodes[it.m_nodes[i + 1]].m_x - iter2.m_x;
					b2 = m_nodes[it.m_nodes[i + 1]].m_y - iter2.m_y;
					a = sqrt(a1 * a1 + a2 * a2);
					b = sqrt(b1 * b1 + b2 * b2);
					grad += acos((a1 * b1 + a2 * b2) / (a * b));
				}
				else
				{
					a1 = m_nodes[it.m_nodes[i]].m_x - iter2.m_x;
					a2 = m_nodes[it.m_nodes[i]].m_y - iter2.m_y;
					b1 = m_nodes[it.m_nodes[i - (it.m_count - 1)]].m_x - iter2.m_x;
					b2 = m_nodes[it.m_nodes[i - (it.m_count - 1)]].m_y - iter2.m_y;
					a = sqrt(a1 * a1 + a2 * a2);
					b = sqrt(b1 * b1 + b2 * b2);
					grad += acos((a1 * b1 + a2 * b2) / (a * b));
				}
			}
			if (grad >= 4 * asin(1))
			{
				result[iter2.m_id] = it.m_cell_id + 1;
			}
		}
	}
	for (const auto& iter : result)
	{
		file << iter << std::endl;
	}
	file.close();
}

void loadmesh::kirkpatrick_points_location()
{
	std::fstream file;
	file.open("kirkpatrick_vivod.txt", std::ios::trunc | std::ios::out);
	file << p_points.size() << std::endl;
	if (!file.is_open())
	{
		throw(std::exception("|  Error: file not found"));
	}
	std::vector<int> result;
	result.reserve(p_points.size());
	bool find = 0;
	for (const auto& iter : p_points)
	{
		m_top_triangle->query(iter, result, find);
		find = 0;
	}
	for (const auto& iter : result)
	{
		file << iter << std::endl;
	}
	file.close();
}