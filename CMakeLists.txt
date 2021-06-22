#***********************************************************
#
#***********************************************************
cmake_minimum_required(VERSION 3.10)
project(QuadTree)

# Set C++ flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

# Define the main module, which also holds the executable
add_executable(QuadTree
               main.cpp)

# Link system libaries
target_link_libraries(QuadTree -lX11)
target_link_libraries(QuadTree -lGL)
target_link_libraries(QuadTree -lpthread)
target_link_libraries(QuadTree -lpng)
target_link_libraries(QuadTree -lstdc++fs)
target_link_libraries(QuadTree -lm)

