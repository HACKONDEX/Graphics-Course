#pragma once

#include <glm/glm.hpp>

#include <objloader.hpp>

#include "default_object.hpp"

class Bone : public DefaultObject<Bone> {
 private:
  enum Detailing {
    kOn,
    kOff,
  };

  enum DetailsLevel {
    High,
    Medium,
    Low,
  };

 public:
  Bone(const glm::vec3& current_look_at)
      : vertices_(default_vertices),
        uvs_(default_uvs),
        normals_(default_normals) {
    spawn_point_ = support::GetPosition();

    glm::vec3 offset = glm::normalize(current_look_at) * 2.0f;
    offset[2] *= -1.0f;
    offset += spawn_point_;

    float angle = HorizontalAngle(current_look_at, kLookAt);
    trans_ = glm::rotate(glm::mat4(1.0f), -angle, glm::vec3(0.0f, 1.0f, 0.0f));

    current_center += offset;
    for (glm::vec3& vertex : vertices_) {
      vertex = glm::vec3(trans_ * glm::vec4(vertex, 1));
      vertex += offset;
    }
    direction = current_center - spawn_point_;
//        current_center = support::GetPosition();
        direction.y = 0.0f;
  }

  static void SwitchDetailing() {
    if (detailing == kOff) {
      detailing = kOn;
    }
    else {
      detailing = kOff;
    }
  }

  void TryChangeDetailing() {
    if (detailing == kOn) {
      if (detail_level_ == High) {
        if (glm::length(current_center - spawn_point_) > 6) {
          SwitchToMedium();
        }
      }
      if (detail_level_ == Medium) {
        if (glm::length(current_center - spawn_point_) > 11) {
          SwitchToLow();
        }
      }
      if (detail_level_ == Low) {
        // pass
      }
    }
  }

  void SwitchToMedium() {
    vertices_.clear();
    vertices_ = default_vertices_medium;
    normals_.clear();
    normals_ = default_normals_medium;
    uvs_.clear();
    uvs_ = default_uvs_medium;
    for (auto& vertex : vertices_) {
      vertex = glm::vec3(trans_ * glm::vec4(vertex, 1));
      vertex += current_center;
    }

    detail_level_ = Medium;
  }

  void SwitchToLow() {
    vertices_.clear();
    vertices_ = default_vertices_low;
    normals_.clear();
    normals_ = default_normals_low;
    uvs_.clear();
    uvs_ = default_uvs_low;
    for (auto& vertex : vertices_) {
      vertex = glm::vec3(trans_ * glm::vec4(vertex, 1));
      vertex += current_center;
    }

    detail_level_ = Low;
  }

  void UpdateLocation() {
    current_center += direction * 0.1f * speed_coef;
    for (glm::vec3& vertex : vertices_) {
      vertex += direction * 0.1f * speed_coef;
    }
  }

  static void LoadObj(const char* filepath) {
    support::LoadObj(filepath, default_vertices, default_uvs, default_normals);
    RotateBoneObj(default_vertices);
  }

  static void LoadLowObj(const char* filepath) {
    support::LoadObj(filepath, default_vertices_low, default_uvs_low,
                     default_normals_low);
    RotateBoneObj(default_vertices_low);
  }

  static void LoadMediumObj(const char* filepath) {
    support::LoadObj(filepath, default_vertices_medium, default_uvs_medium,
                     default_normals_medium);
    RotateBoneObj(default_vertices_medium);
  }

 public:
  std::vector<glm::vec3> vertices_;
  std::vector<glm::vec3> normals_;
  std::vector<glm::vec2> uvs_;
  glm::vec3 current_center{0, 0, 0};
  glm::vec3 direction;
  glm::vec3 spawn_point_;
  glm::mat4 trans_;
  static inline const float kDistanceToCollide = 0.35f;

  DetailsLevel detail_level_{High};

 private:
  static void RotateBoneObj(std::vector<glm::vec3>& vertices) {
    glm::mat4 trans = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f),
                                  glm::vec3(0.0f, 1.0f, 0.0f));

    for (glm::vec3& vertex : vertices) {
      vertex = glm::vec3(trans * glm::vec4(vertex, 1));
    }
  }

  float HorizontalAngle(glm::vec3 v1, glm::vec3 v2) {
    v1[1] = 0;
    v2[1] = 0;
    float angle = acos(glm::dot(glm::normalize(v1), glm::normalize(v2)));
    if (v1[0] * v2[2] - v1[2] * v2[0] < 0) {
      angle *= -1;
    }
    return angle;
  }

 private:
  static inline const glm::vec3 kLookAt{0.0f, 0.0f, -1.0f};

  static inline std::vector<glm::vec3> default_vertices_low;
  static inline std::vector<glm::vec3> default_normals_low;
  static inline std::vector<glm::vec2> default_uvs_low;

  static inline std::vector<glm::vec3> default_vertices_medium;
  static inline std::vector<glm::vec3> default_normals_medium;
  static inline std::vector<glm::vec2> default_uvs_medium;

  static inline Detailing detailing{kOff};
};
