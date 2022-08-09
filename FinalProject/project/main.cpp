#include <vector>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <texture_loader.hpp>
#include <controls.hpp>

#include "game_manager.hpp"
#include "gl_manager.hpp"
#include "ground.hpp"
#include "text_manager.hpp"
#include "heart.hpp"
#include "sky.hpp"

using Colors = support::GlManager::BackgroundColors;

GameManager game_manager;

void KeyCallbacks(GLFWwindow*, int key, int, int action, int) {
  if (key == GLFW_KEY_T && action == GLFW_PRESS) {
    game_manager.IncreaseSpeedCoef();
  } else if (key == GLFW_KEY_G && action == GLFW_PRESS) {
    game_manager.DecreaseSpeedCoef();
  } else if (key == GLFW_KEY_R && action == GLFW_PRESS) {
    game_manager.SwitchBonesDetailing();
  }
}

int main() {
  support::GlManager manager;

  if (!manager.InitialiseGLFW() || !manager.MakeWindow("FeedTheDoggo3D") ||
      !manager.InitialiseGLEW()) {
    std::cout << "Failed to initialize. Closing...";
    return 1;
  }

  manager.CheckEscKey();
  manager.SetBackgroundColor(Colors::kLightBlue);
  manager.EnableLessDepthTest();
  manager.SetupCursor();
  glEnable(GL_CULL_FACE);

  GLuint vertex_array_id;
  glGenVertexArrays(1, &vertex_array_id);
  glBindVertexArray(vertex_array_id);

  // Compile shader programmes.
  GLuint program_id = manager.LoadShaders("VertexStandardShading.glsl",
                                          "FragmentStandardShading.glsl");

  // Get a handle for our "MVP" variable from shader.
  GLuint mvp_matrix_id = glGetUniformLocation(program_id, "MVP");
  GLuint view_matrix_id = glGetUniformLocation(program_id, "V");
  GLuint model_matrix_id = glGetUniformLocation(program_id, "M");
  GLuint light_color_id = glGetUniformLocation(program_id, "LightColor");
  GLuint light_power_id = glGetUniformLocation(program_id, "LightPower");

  // Load textures for doggos and bones.
  GLuint bone_texture = support::LoadDds("bone_texture.dds");
  GLuint doggo_texture = support::LoadDds("doggo_texture.dds");
  GLuint heart_texture = support::LoadDds("heart_texture.dds");
  GLuint ground_texture = support::LoadDds("ground_texture.dds");
  GLuint sky_texture = support::LoadDds("sky_texture.dds");

  // Get a handle for our "myTextureSampler" variable from shader.
  GLuint texture_id = glGetUniformLocation(program_id, "myTextureSampler");

  // Read our obj files to set default vertices of our objects.
  Doggo::LoadObj("doggo.obj");
  Heart::LoadObj("heart.obj");
  Bone::LoadObj("bone.obj");
  Bone::LoadLowObj("boneLow.obj");
  Bone::LoadMediumObj("boneMedium.obj");
  Ground::LoadObj("ground.obj");
  Sky::LoadObj("sky.obj");

  // Spawn starting doggos
  game_manager.AddGround();
  game_manager.AddSky();
  game_manager.SetupScene();

  GLuint light_id =
      glGetUniformLocation(program_id, "LightPosition_worldspace");
  game_manager.SetLight(light_id, light_color_id, light_power_id);

  TextManager text_manager("Holstein.DDS", "TextVertexShader.glsl",
                           "TextFragmentShader.glsl");

  manager.SetKeyCallback(KeyCallbacks);
  while (!manager.ShouldQuit()) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(program_id);

    // Compute the MVP matrix from keyboard and mouse input.
    support::ComputeMatricesFromInputs(manager.GetWindow());
    glm::mat4 projection_matrix = support::GetProjectionMatrix();
    glm::mat4 view_matrix = support::GetViewMatrix();
    glm::mat4 model_matrix = glm::mat4(1.0);
    glm::mat4 mvp = projection_matrix * view_matrix * model_matrix;

    // Get the 3-rd column - the forward direction for bones direction.
    game_manager.SetCurrentLookAt(glm::vec3(view_matrix[2]));

    game_manager.TryToAddDoggo();
    if (manager.IsPressed(GLFW_MOUSE_BUTTON_LEFT)) {
      game_manager.TryToThrowABone();
    }
    game_manager.UpdateBonesLocation();

    game_manager.UpdateHeartsLocations();

    game_manager.UpdateLightLocation();

    // Move mvp data to shaders.
    glUniformMatrix4fv(mvp_matrix_id, 1, GL_FALSE, &mvp[0][0]);
    glUniformMatrix4fv(model_matrix_id, 1, GL_FALSE, &model_matrix[0][0]);
    glUniformMatrix4fv(view_matrix_id, 1, GL_FALSE, &view_matrix[0][0]);

    game_manager.DisplayLight();

    // Set texture for Doggos.
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, doggo_texture);
    glUniform1i(texture_id, 0);

    game_manager.DrawDoggos(manager);

    // Set texture for Bones.
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, bone_texture);
    glUniform1i(texture_id, 0);

    game_manager.DrawBones(manager);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ground_texture);
    glUniform1i(texture_id, 0);

    game_manager.DrawGround(manager);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sky_texture);
    glUniform1i(texture_id, 0);

    game_manager.DrawSky(manager);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, heart_texture);
    glUniform1i(texture_id, 0);

    game_manager.DrawHearts(manager);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    text_manager.Display(game_manager.GetDoggosFedCount(),
                         game_manager.GetSpeedSlowdown(),
                         game_manager.GetDaysCount());

    // Swap buffers
    glfwSwapBuffers(manager.GetWindow());
    glfwPollEvents();
  }

  // Cleanup VBO and shader

  glDeleteTextures(1, &bone_texture);
  glDeleteTextures(1, &doggo_texture);
  glDeleteTextures(1, &heart_texture);
  glDeleteTextures(1, &ground_texture);
  glDeleteVertexArrays(1, &vertex_array_id);
  return 0;
}