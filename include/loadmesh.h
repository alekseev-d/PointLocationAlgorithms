#pragma once
#include <vector>
#include <string>
#include "cells.h"

class loadmesh																				// ��������� �����
{
	node_vec m_nodes;																		// ���� �����
	std::vector<cell> m_cells;																// ������ �����												
	cell_ptr m_top_triangle;																// �������� ������ ������������

	node_vec p_points;																		// ����� ��� ��������
public:
	loadmesh(const std::string& p_str);														// �������� �����, ������������

	void points_location();																	// �������

	void kirkpatrick_points_location();														// ����� �����������
};