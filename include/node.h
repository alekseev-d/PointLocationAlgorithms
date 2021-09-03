#pragma once
#include <vector>

struct node
{
	double m_x, m_y;
	int m_id;

	static int nCount;

	node() : m_x(0), m_y(0), m_id(0) {}
	node(double x, double y) : m_x(x), m_y(y) { ++nCount; m_id = nCount; }
	node& operator += (const node& p_node)
	{
		m_x += p_node.m_x;
		m_y += p_node.m_y;
		return *this;
	}
};

class edge
{
	node m_beg, m_end;
public:
	edge() = default;
	edge(const node& beg, const node& end) : m_beg(beg), m_end(end) {}
	const node& operator[] (int i) const
	{
		if (i == 0)
		{
			return m_beg;
		}
		else
		{
			return m_end;
		}
	}
};

inline bool operator < (const node& p_node1, const node& p_node2)
{
	if (p_node1.m_x == p_node2.m_x)
	{
		return p_node1.m_y < p_node2.m_y;
	}
	else
	{
		return p_node1.m_x < p_node2.m_x;
	}
}

inline bool operator == (const node& p_node1, const node& p_node2)
{
	return (p_node1.m_x == p_node2.m_x) && (p_node1.m_y == p_node2.m_y);
}

struct cell
{
	std::vector<int> m_nodes;
	int m_count;
	int m_cell_id;

	cell() : m_count(0), m_cell_id(0) {}
};
