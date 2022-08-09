#pragma once

namespace support {

// Load a .BMP file using our custom loader
GLuint LoadBmp(const char* imagepath);

// Load a .DDS file using GLFW's own loader
GLuint LoadDds(const char* imagepath);

}  // namespace support