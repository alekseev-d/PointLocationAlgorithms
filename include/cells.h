#pragma once
#include "util.h"
#include <memory>
#include "node.h"
#include <set>

struct cells;
typedef std::shared_ptr<cells> cell_ptr;
typedef std::set<std::shared_ptr<cells> > cell_set;

struct cells
{
	int m_id;
	node_vec m_cells_nodes;
	std::set<cell_ptr> m_children;
	bool m_is_inside;

	static int sCount;

	cells();
	cells(const node_vec& p_cells_nodes, bool p_is_inside);
	~cells();
	bool inside(const node& p_node) const;
	bool query(const node& p_node, std::vector<int>& p_vec, bool &find);
	void add_child(const cell_ptr& p_child);
	bool adding(const cell_ptr& p_cell); 
};

bool intersects(const cells& p_cell1, const cells& p_cell2);
