#pragma once
template <typename T>
class DefaultObject {
 public:
  static void IncreaseSpeedBy(float factor) {
    speed_coef *= factor;
  }

  static void DecreaseSpeedBy(float factor) {
    speed_coef /= factor;
  }
 protected:
  static inline std::vector<glm::vec3> default_vertices;
  static inline std::vector<glm::vec3> default_normals;
  static inline std::vector<glm::vec2> default_uvs;

  static inline float speed_coef = 1.0f;
};