#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
using namespace glm;

#include <common/shader.hpp>

int main(void) {
    // Initialise GLFW
    if (!glfwInit()) {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow( 1024, 768, "Tutorial 02 - Red triangle", NULL, NULL);
    if (window == NULL) {
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);




    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Dark blue background
//    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Create and compile our GLSL program from the shaders
    GLuint programID_1 = LoadShaders( "SimpleVertexShader.vertexshader", "SimpleFragmentShaderTriangle_1.fragmentshader" );


    static const GLfloat g_vertex_buffer_data_1[] = {
            0.0f, 0.9f, 0.0f,
            -0.75f, -0.5f, 0.0f,
            0.75f,  -0.5f, 0.0f,
    };

    GLuint vertexbuffer_1;
    glGenBuffers(1, &vertexbuffer_1);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data_1), g_vertex_buffer_data_1, GL_STATIC_DRAW);

    GLuint programID_2 = LoadShaders( "SimpleVertexShader.vertexshader", "SimpleFragmentShaderTriangle_2.fragmentshader" );

    static const GLfloat g_vertex_buffer_data_2[] = {
            0.0f, -0.9f, 0.0f,
            -0.75f, 0.5f, 0.0f,
            0.75f,  0.5f, 0.0f,
    };

    GLuint vertexbuffer_2;
    glGenBuffers(1, &vertexbuffer_2);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data_2), g_vertex_buffer_data_2, GL_STATIC_DRAW);

    // my background
//    glClearColor(0.956f, 0.949f, 0.643f, 1.0f);
        glClearColor(0.921f, 0.929f, 0.956f, 1.0f);
//    glClear(GL_COLOR_BUFFER_BIT);


    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
    glBlendEquation(GL_FUNC_ADD);

    do{

        // Clear the screen
        glClear( GL_COLOR_BUFFER_BIT );

        // Use our shader
        glUseProgram(programID_1);

        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_1);
        glVertexAttribPointer(
                0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
        );

        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle

        glUseProgram(programID_2);



        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_2);
        glVertexAttribPointer(
                0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
        );

        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle

        glDisableVertexAttribArray(0);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

    // Cleanup VBO
    glDeleteBuffers(1, &vertexbuffer_1);
    glDeleteVertexArrays(1, &VertexArrayID);
    glDeleteProgram(programID_1);

    glDeleteBuffers(1, &vertexbuffer_2);
    glDeleteProgram(programID_2);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}

