#pragma once

#include <glm/glm.hpp>

#include "default_object.hpp"

class Heart : public DefaultObject<Heart> {
 public:
  Heart(glm::vec3& offset, glm::mat4& rotate)
      : spawn_point_(offset),
        vertices_(default_vertices),
        normals_(default_normals),
        uvs_(default_uvs) {
    current_center += spawn_point_;
    for (auto& vertex : vertices_) {
      vertex = glm::vec3(rotate * glm::vec4(vertex, 1));
      vertex += offset;
    }
  }

  void UpdateLocation() {
    current_center += direction * speed_coef;
    if (!blown_) {
      for (auto& vertex : vertices_) {
        vertex += direction * speed_coef;
      }
    } else {
      for (size_t i = 0; i < vertices_.size(); ++i) {
        if (i >= blow_directions.size()) {
          std::cout << "Something went wrong in Heart, we should have "
                       "normals.size = vertices.size"
                    << std::endl;
          std::abort();
        }
        vertices_[i] += blow_directions[i];
      }
    }
  }

  void Blow() {
    blown_ = true;
    for (auto& normal : normals_) {
      float norma = glm::length(normal);
      float scale = norma;
      blow_directions.emplace_back((normal / scale) * 0.1f * speed_coef);
    }
  }

  bool IsBlown() const {
    return blown_;
  }

  static void LoadObj(const char* filepath) {
    support::LoadObj(filepath, default_vertices, default_uvs, default_normals);
  }

 public:
  glm::vec3 spawn_point_;
  std::vector<glm::vec3> vertices_;
  std::vector<glm::vec3> normals_;
  std::vector<glm::vec2> uvs_;
  glm::vec3 current_center{0, 0, 0};
  bool blown_ = false;
  std::vector<glm::vec3> blow_directions;

 private:
  static inline glm::vec3 direction{0, 0.05f, 0};
};