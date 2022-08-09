#include "shader_loader.hpp"

namespace support {

namespace detail {

std::string ReadFile(const char* filepath) {
  std::string code;
  std::ifstream shader_stream(filepath, std::ios::in);
  if (shader_stream.is_open()) {
    std::stringstream sstr;
    sstr << shader_stream.rdbuf();
    code = sstr.str();
    shader_stream.close();
  }
  return code;
}

void CompileShader(const char* filepath, std::string& code, GLuint& shader_id) {
  printf("Compiling shader : %s\n", filepath);
  char const* vertex_source_pointer = code.c_str();
  glShaderSource(shader_id, 1, &vertex_source_pointer, nullptr);
  glCompileShader(shader_id);
}

void CheckProgram(GLuint& shader_id, GLint& result) {
  int info_log_length;
  glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
  glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
  if (info_log_length > 0) {
    std::vector<char> vertex_shader_error_message(info_log_length + 1);
    glGetShaderInfoLog(shader_id, info_log_length, nullptr,
                       &vertex_shader_error_message[0]);
    printf("%s\n", &vertex_shader_error_message[0]);
  }
}

bool MakeShader(const char* shader_path, GLuint& shader_id, GLint& result) {
  std::string shader_code = ReadFile(shader_path);
  if (shader_code.empty()) {
    printf("Wrong path to the shader specified %s", shader_path);
    getchar();
    return false;
  }
  // Compile and Check Vertex Shader
  CompileShader(shader_path, shader_code, shader_id);
  CheckProgram(shader_id, result);
  return true;
}

}  // namespace detail

GLuint LoadShaders(const char* vertex_file_path,
                   const char* fragment_file_path) {
  // Create the shaders
  GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
  GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

  GLint result = GL_FALSE;

  // Try to compile 2 shaders. Return 0 on error.
  if (!detail::MakeShader(vertex_file_path, vertex_shader_id, result) ||
      !detail::MakeShader(fragment_file_path, fragment_shader_id, result)) {
    return 0;
  }

  // Link the program
  printf("Linking program\n");
  GLuint program_id = glCreateProgram();
  glAttachShader(program_id, vertex_shader_id);
  glAttachShader(program_id, fragment_shader_id);
  glLinkProgram(program_id);

  // Check the program
  detail::CheckProgram(program_id, result);

  glDetachShader(program_id, vertex_shader_id);
  glDetachShader(program_id, fragment_shader_id);

  glDeleteShader(vertex_shader_id);
  glDeleteShader(fragment_shader_id);

  return program_id;
}

}  // namespace framework
