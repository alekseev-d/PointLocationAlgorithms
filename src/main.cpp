#include "loadmesh.h"
#include <time.h>
#include <iostream>

int cells::sCount = 0;
int node::nCount = 0;

int main(int argc, char** argv)
{
	std::cout << std::endl << "Point Location Application" << std::endl;

	if (argc > 1)
	{
		if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)
		{
			std::cout << "|  Usage: point_location.exe --path file_path_to_mesh.txt" << std::endl;
		}
		else if (strcmp(argv[1], "--path") == 0)
		{
			try
			{
				loadmesh test(argv[2]);

				clock_t start = clock();
				test.points_location();
				clock_t end = clock();

				std::cout << "|  Brute force point location time: " << (double)(end - start) / CLOCKS_PER_SEC << std::endl;

				start = clock();
				test.kirkpatrick_points_location();
				end = clock();

				std::cout << "|  Kirkpatrick point location time: " << (double)(end - start) / CLOCKS_PER_SEC << std::endl;
			}
			catch (const std::exception& e)
			{
				std::cout << e.what() << std::endl;
			}
		}
		else
		{
			std::cout << "|  Error: invalid application arguments" << std::endl
				<< "|  Use --help (-h) for help information" << std::endl;
		}
	}
	else
	{
		std::cout << "|  Error: invalid application arguments" << std::endl
			<< "|  Use --help (-h) for help information" << std::endl;
	}

	std::cout << "Goodbye!" << std::endl;

	return 0;
}
