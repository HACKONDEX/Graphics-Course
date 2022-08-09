#pragma once

#include <text2D.hpp>

class TextManager {
 public:
  TextManager(const char* path, const char* text_vertex_shader,
              const char* text_fragment_shader) {
    support::InitText2D(path, text_vertex_shader, text_fragment_shader);
  }

  ~TextManager() {
    // Delete the text's VBO, the shader and the texture
    support::CleanupText2D();
  }

  void Display(uint64_t number, float slowdown, int64_t days_count) {
    PrintFedCounter(number);
    PrintCursor();
    PrintSpeedSlowdown(slowdown);
    PrintDaysCount(days_count);
  }


 private:
  void PrintFedCounter(uint64_t number) {
    std::string bone("Doggos fed: " + std::to_string(number));
    support::PrintText2D(bone.data(), /*x=*/0, /*y=*/550, /*size=*/25);
  }

  void PrintCursor() {
    std::string cursor("+");
    support::PrintText2D(cursor.data(), /*x=*/380, /*y=*/275, /*size=*/50);
  }

  void PrintSpeedSlowdown(float slowdown) {
    std::string speed("Speed: " + std::to_string(slowdown).substr(0, 4));
    support::PrintText2D(speed.data(), /*x=*/0, /*y=*/520, /*size=*/25);
  }

  void PrintDaysCount(int64_t count) {
    std::string days("Days: " + std::to_string(count));
    support::PrintText2D(days.data(), /*x=*/0, /*y=*/490, /*size=*/25);
  }
};