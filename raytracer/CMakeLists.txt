cmake_minimum_required(VERSION 3.10)

# Project name
project(ImageRenderer)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find MPI
find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})

# Ensure compiler uses C++17 (or later) explicitly
add_compile_options(-std=c++17)

# Add the executable target
add_executable(image_renderer main.cpp)

# Link against MPI (if needed)
target_link_libraries(image_renderer MPI::MPI_CXX)
