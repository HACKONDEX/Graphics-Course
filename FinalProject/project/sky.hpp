#pragma once

#include "default_object.hpp"

class Sky : public DefaultObject<Sky> {
 public:
  Sky()
      : vertices_(default_vertices),
        normals_(default_normals),
        uvs_(default_uvs) {
  }

  static void LoadObj(const char* filepath) {
    support::LoadObj(filepath, default_vertices, default_uvs, default_normals);
  }

 public:
  std::vector<glm::vec3> vertices_;
  std::vector<glm::vec3> normals_;
  std::vector<glm::vec2> uvs_;
};
