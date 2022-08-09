cmake_minimum_required(VERSION 3.18)
project(computer_graphics)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
add_subdirectory(external)
include_directories(
	external/glfw-3.1.2/include/
	external/glm-0.9.7.1/
	external/glew-1.13.0/include/)

set(ALL_EXTERNAL_LIBS ${OPENGL_LIBRARY} glfw GLEW_1130)


add_library(GLSupport
    support/shader_loader.cpp
    support/gl_manager.cpp
    support/controls.cpp
    support/objloader.cpp
    support/text2D.cpp
    support/texture_loader.cpp
)

include_directories(support)
link_libraries(${ALL_EXTERNAL_LIBS} GLSupport)
add_subdirectory(project)
