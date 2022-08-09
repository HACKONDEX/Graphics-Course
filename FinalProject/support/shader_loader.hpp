#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>
#include <sstream>
#include <fstream>
#include <vector>

namespace support {

namespace detail {
std::string ReadFile(const char* filepath);

void CompileShader(const char* filepath, std::string& code, GLuint& shader_id);

void CheckProgram(GLuint& shader_id, GLint& result);

bool MakeShader(const char* shader_path, GLuint& shader_id, GLint& result);

}  // namespace detail

GLuint LoadShaders(const char* vertex_file_path,
                   const char* fragment_file_path);

}  // namespace framework
