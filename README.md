# Task
Given a planar subdivision, formed by multiple polygons called cells, and list of query points. 

Need to determine which cell contains a query points with 2 algorithms:
* 1) brute force search of each cell using the point-in-polygon algorithm,
* 2) Kirkpatrick's Algorithm for log(n) point location in planar subdivisions.

Usage:
```
point_location.exe --path path_to_mesh_file.txt
```
