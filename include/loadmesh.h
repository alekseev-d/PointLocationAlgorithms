#pragma once
#include <vector>
#include <string>
#include "cells.h"

class loadmesh																				// Загрузчик сетки
{
	node_vec m_nodes;																		// Узлы сетки
	std::vector<cell> m_cells;																// Ячейки сетки												
	cell_ptr m_top_triangle;																// Бинарное дерево триангуляции

	node_vec p_points;																		// Точки для проверки
public:
	loadmesh(const std::string& p_str);														// Загрузка сетки, триангуляция

	void points_location();																	// Перебор

	void kirkpatrick_points_location();														// Метод Киркпатрика
};