cmake_minimum_required(VERSION 3.0)
project(BandBMCP)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(BMCP)

add_executable(Solver main.cpp)

target_link_libraries(Solver BMCP)