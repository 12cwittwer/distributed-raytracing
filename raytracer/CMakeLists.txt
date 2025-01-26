# Minimum required CMake version
cmake_minimum_required(VERSION 3.10)

# Project name
project(ImageRenderer)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable target
add_executable(image_renderer main.cpp)