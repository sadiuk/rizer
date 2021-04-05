cmake_minimum_required(VERSION 3.17)

project(rizer)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)

add_subdirectory(third_party)
add_subdirectory(examples)

add_library(rizer src/Rasterizer.cpp
				  src/Rasterizer.h)

target_include_directories(rizer PUBLIC third_party/glm third_party/glad/include third_party/glfw/include)

