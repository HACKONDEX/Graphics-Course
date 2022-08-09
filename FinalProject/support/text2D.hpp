#pragma once

namespace support {

void InitText2D(const char* texture_path, const char* text_vertex_shader,
                const char* text_fragment_shader);
void PrintText2D(const char* text, int x, int y, int size);
void CleanupText2D();
}  // namespace support