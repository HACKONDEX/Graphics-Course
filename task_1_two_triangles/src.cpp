#include <stdio.h>
#include <stdlib.h>

/// GLEW
#include <GL/glew.h>

/// GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

/// GLM
#include <glm/glm.hpp>
using namespace glm;

#include <common/shader.hpp>

int main(void)
{
    // Initialise GLFW
    if(!glfwInit()) {
	fprintf( stderr, "Failed to initialize GLFW\n" );
	getchar();
	return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    return 0;
}
