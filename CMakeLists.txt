cmake_minimum_required(VERSION 3.17)

project(rizer)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)

add_subdirectory(third_party)
add_subdirectory(examples)

add_library(rizer include/App.cpp
				  include/App.h
				  src/OpenGL/Buffer.h
				  src/OpenGL/Buffer.cpp
				  src/OpenGL/Program.h
				  src/OpenGL/ComputeProgram.cpp
				  src/OpenGL/ComputeProgram.h
				  src/OpenGL/Texture2D.cpp
				  src/OpenGL/Texture2D.h
				  src/OpenGL/Framebuffer.cpp
				  src/OpenGL/Framebuffer.h
				  
				  src/fs/FileManager.cpp
				  src/fs/FileManager.h
				  src/fs/OBJLoader.h

				  src/math/vector.h
				  
				  src/GLContext.h
				  src/GLContext.cpp
				  src/Rasterizer.h
				  src/Rasterizer.cpp
				  src/FullScreenRenderer.cpp
				  src/FullScreenRenderer.h
				  src/Debug.h)

target_include_directories(rizer PUBLIC third_party/glm third_party/glad/include third_party/glfw/include third_party/imgui)

target_link_libraries(rizer glad glfw imgui)