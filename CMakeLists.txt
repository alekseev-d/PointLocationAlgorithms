cmake_minimum_required(VERSION 3.18)

project(point_location LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/cells.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/graph.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/kirkpatrick_func.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/loadmesh.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/node.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/util.h
    )

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/cells.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/graph.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/loadmesh.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    )

add_executable(point_location ${SOURCES} ${HEADERS})