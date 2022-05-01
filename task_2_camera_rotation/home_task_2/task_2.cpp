#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

GLFWwindow *window;

// Include GLM
#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>


using namespace glm;

#include <common/shader.hpp>

enum Parameters {
    WindowLength = 1048,
    WindowWidth = 768,
};

static void InitializeGLFW() {
    // Initialise GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        exit(-1);
    }
}

static void SetupWindow() {
    glfwWindowHint(GLFW_SAMPLES, 4); // Smoothing x4
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Version control
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(WindowLength, WindowWidth, "Task 1 two triangles", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr,
                "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible.\n");
        getchar();
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(window);
}

static void InitializeGLEW() {
    // Initialize GLEW
    glewExperimental = true; // For core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        exit(-1);
    }
}

static void SetKeyBinds() {
    // Ensure we can capture the escape key being pressed
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
}

static void SetBackground() {
    glClearColor(0.921f, 0.929f, 0.956f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

static void EnableBlending() {
    glEnable(GL_BLEND);
    // Specifies how to compute RGB and alpha
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

int main(void) {
    InitializeGLFW();

    SetupWindow();

    InitializeGLEW();

    SetKeyBinds();

    SetBackground();

    EnableBlending();

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Create and compile our GLSL program from the shaders
    GLuint programID_1 = LoadShaders("SimpleVertexShader.vertexshader",
                                     "SimpleFragmentShaderTriangle_1.fragmentshader");
    GLuint programID_2 = LoadShaders("SimpleVertexShader.vertexshader",
                                     "SimpleFragmentShaderTriangle_2.fragmentshader");

    GLuint MatrixID = glGetUniformLocation(programID_2, "MVP");
    glm::mat4 Projection = glm::perspective(
            // Вертикальное поле зрения в радианах. Обычно между 90&deg; (очень широкое) и 30&deg; (узкое)
            glm::radians(35.0f),
            // Отношение сторон. Зависит от размеров вашего окна. Заметьте, что 4/3 == 800/600 == 1280/960
            8.0f / 5.0f,
            // Ближняя плоскость отсечения. Должна быть больше 0
            0.1f,
            // Дальняя плоскость отсечения.
            100.0f);
    glm::mat4 View = glm::lookAt(
            // Камера находится в мировых координатах (4,3,3)
            glm::vec3(4, 3, 3),
            // И направлена в начало координат
            glm::vec3(0, 0, 0),
            // "Голова" находится сверху
            glm::vec3(0, 1, 0)
    );
    glm::mat4 Model = glm::mat4(1.0f);
    // Итоговая матрица ModelViewProjection, которая является результатом перемножения наших трех матриц
    glm::mat4 MVP = Projection * View * Model; // Помните, что умножение матрицы производиться в обратном порядке

    const double PI_2_Coef = 2 * glm::pi<double>() / 500;

    // Coordinates
    static const GLfloat g_vertex_buffer_data[] = {
            0.0f, 0.9f, 0.0f,
            -0.75f, -0.5f, 0.0f,
            0.75f, -0.5f, 0.0f,

            0.0f, -0.9f, 0.0f,
            -0.75f, 0.5f, 0.0f,
            0.75f, 0.5f, 0.0f,
    };

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);


    uint32_t time = 0;

    do {

        View = glm::lookAt(
                glm::vec3(
                        5 * glm::sin( PI_2_Coef * time),
                        3,
                        5 * glm::cos(PI_2_Coef * time)
                ),
                glm::vec3(0, 0, 0),
                glm::vec3(0, 1, 0)
        );
        ++time;
        MVP = Projection * View * Model;

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(programID_1);
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(programID_2);
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glDrawArrays(GL_TRIANGLES, 3, 6);
        glDisableVertexAttribArray(0);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
             glfwWindowShouldClose(window) == 0);

    // Cleanup VBO
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteVertexArrays(1, &VertexArrayID);
    glDeleteProgram(programID_1);
    glDeleteProgram(programID_2);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}
