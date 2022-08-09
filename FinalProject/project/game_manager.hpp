#pragma once

#include <algorithm>
#include <optional>

#include <gl_manager.hpp>

#include "bone.hpp"
#include "doggo.hpp"
#include "ground.hpp"
#include "heart.hpp"
#include "sky.hpp"
#include "light.hpp"


class GameManager {
  static inline const int kMaxDistance = 30;
  static inline const int kEnemiesCountOnStart = 5;
  static inline const int kMaxEnemiesCount = 20;
  static inline const double kDoggoSpawnDelay = 2.0;
  static inline const double kBoneDelay = 0.5;
  static inline const float kSpeedChange = 0.5f;

 public:
  void SetupScene() {
    for (size_t i = 0; i < kEnemiesCountOnStart; ++i) {
      CreateNewDoggo();
    }
  }

  void SwitchBonesDetailing() {
    Bone::SwitchDetailing();
  }

  void UpdateBonesLocation() {
//    std::remove_if(bones_.begin(), bones_.end(), [](const Bone& bone) {
//      return glm::length(bone.current_center) > kMaxDistance;
//    });

    for (size_t i = 0; i < bones_.size(); ++i) {
      if (glm::length(bones_[i].current_center) > kMaxDistance) {
        bones_.erase(bones_.begin() + i);
        continue ;
      }
      ++i;
    }

    std::for_each(bones_.begin(), bones_.end(), [](Bone& bone) {bone.TryChangeDetailing();});

    std::for_each(bones_.begin(), bones_.end(),
                  [](Bone& bone) { bone.UpdateLocation(); });
    TryRemoveCollidedObjects();
  }

  void UpdateHeartsLocations() {
    for (size_t i = 0; i < hearts_.size();) {
      if (glm::length(hearts_[i].current_center - hearts_[i].spawn_point_) > 10) {
        hearts_.erase(hearts_.begin() + i);
        continue ;
      }
      ++i;
    }

    std::for_each(hearts_.begin(), hearts_.end(), [](Heart& heart) {
      if (!heart.IsBlown() && glm::length(heart.current_center - heart.spawn_point_) > 7) {
        heart.Blow();
      }
    });

    std::for_each(hearts_.begin(), hearts_.end(), [](Heart& heart) {
      heart.UpdateLocation();
    });
  }

  void UpdateLightLocation() {
    days_count_ += light_->UpdatePosition();
  }

  void DrawBones(support::GlManager& manager) {
    MakeContiguousBones();
    Draw(manager, bones_vertices_, bones_normals_, bones_uvs_);
  }

  void DrawDoggos(support::GlManager& manager) {
    MakeContiguousDoggos();
    Draw(manager, doggos_vertices_, doggos_normals_, doggos_uvs_);
  }

  void DrawHearts(support::GlManager& manager) {
    MakeContiguousHeartsAndDraw(manager);
  }

  void DrawGround(support::GlManager& manager) {
    assert(ground_.has_value());
    Draw(manager, ground_->vertices_, ground_->normals_, ground_->uvs_);
  }

  void IncreaseSpeedCoef() {
    speed_slowdown_ *= kSpeedChange;
    Bone::IncreaseSpeedBy(kSpeedChange);
    Heart::IncreaseSpeedBy(kSpeedChange);
    support::IncreaseMovementSpeedCoefBy(kSpeedChange);
    light_->IncreaseSpeedBy(kSpeedChange);
  }

  void DecreaseSpeedCoef() {
    speed_slowdown_ /= kSpeedChange;
    Bone::DecreaseSpeedBy(kSpeedChange);
    Heart::DecreaseSpeedBy(kSpeedChange);
    support::DecreaseMovementSpeedCoefBy(kSpeedChange);
    light_->DecreaseSpeedBy(kSpeedChange);
  }

  void DrawSky(support::GlManager& manager) {
    Draw(manager, sky_->vertices_, sky_->normals_, sky_->uvs_);
  }

  void DisplayLight() {
    light_->Display();
  }

  void AddGround() {
    ground_.emplace(Ground());
  }

  void AddSky() {
    sky_.emplace(Sky());
  }

  void SetLight(GLuint program_id, GLuint color_id, GLuint power_id) {
    light_.emplace(Light(program_id, color_id, power_id));
  }

  void TryToAddDoggo() {
    double cur_time = glfwGetTime();
    if (cur_time - last_doggo_time_ > kDoggoSpawnDelay) {
      last_doggo_time_ = cur_time;
      if (doggos_count_ < kMaxEnemiesCount) {
        CreateNewDoggo();
      }
    }
  }

  void TryToThrowABone() {
    double cur_time = glfwGetTime();
    if (cur_time - last_bone_time_ > kBoneDelay) {
      last_bone_time_ = cur_time;
      ThrowABone();
    }
  }

  void ThrowABone() {
    ++bones_count_;
    bones_.emplace_back(cur_look_at_);
  }

  void SetCurrentLookAt(glm::vec3 other) {
    cur_look_at_ = other;
  }

  // Getters.
  //////////////////////////////////////////////////////////////////////////////
  int GetDoggosFedCount() const {
    return doggo_fed_;
  }

  float GetSpeedSlowdown() const {
    return speed_slowdown_;
  }

  int64_t GetDaysCount() const {
    return days_count_;
  }

 private:
  void CreateNewDoggo() {
    if (doggos_count_ + 1 >= kMaxEnemiesCount) {
      return;
    }

    ++doggos_count_;
    doggos_.emplace_back(Doggo());
  }

  void RemoveDoggo(size_t k) {
    --doggos_count_;
    doggos_.erase(doggos_.begin() + k);
  }

  void RemoveBone(size_t k) {
    --bones_count_;
    bones_.erase(bones_.begin() + k);
  }

  void TryRemoveCollidedObjects() {
    for (size_t i = 0; i < bones_.size(); ++i) {
      for (size_t j = 0; j < doggos_.size(); ++j) {
        if (glm::distance(bones_[i].current_center, doggos_[j].current_center) <
            Bone::kDistanceToCollide + Doggo::kDistanceToCollide) {
          ++doggo_fed_;
          RemoveBone(i);
          CreateHeart(doggos_[j]);
          RemoveDoggo(j);
          --i;
          break;
        }
      }
    }
  }

  void CreateHeart(Doggo& doggo) {
    hearts_.push_back(Heart(doggo.current_center, doggo.rotate_));
  }

  void MakeContiguousDoggos() {
    doggos_vertices_.clear();
    doggos_normals_.clear();
    doggos_uvs_.clear();
    for (Doggo& doggo : doggos_) {
      doggos_vertices_.insert(doggos_vertices_.end(), doggo.vertices_.begin(),
                              doggo.vertices_.end());
      doggos_normals_.insert(doggos_normals_.end(), doggo.normals_.begin(),
                             doggo.normals_.end());
      doggos_uvs_.insert(doggos_uvs_.end(), doggo.uvs_.begin(),
                         doggo.uvs_.end());
    }
  }

  void MakeContiguousBones() {
    bones_vertices_.clear();
    bones_normals_.clear();
    bones_uvs_.clear();
    for (Bone& bone : bones_) {
      bones_vertices_.insert(bones_vertices_.end(), bone.vertices_.begin(),
                             bone.vertices_.end());
      bones_normals_.insert(bones_normals_.end(), bone.normals_.begin(),
                            bone.normals_.end());
      bones_uvs_.insert(bones_uvs_.end(), bone.uvs_.begin(), bone.uvs_.end());
    }
  }

  void MakeContiguousHeartsAndDraw(support::GlManager& manager) {
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;
    for (auto& heart : hearts_) {
      vertices.insert(vertices.end(), heart.vertices_.begin(),
                      heart.vertices_.end());
      normals.insert(normals.end(), heart.normals_.begin(),
                     heart.normals_.end());
      uvs.insert(uvs.end(), heart.uvs_.begin(), heart.uvs_.end());
    }
    Draw(manager, vertices, normals, uvs);
  }

  void Draw(support::GlManager& manager, std::vector<glm::vec3>& vertices,
            std::vector<glm::vec3>& normals, std::vector<glm::vec2>& uvs) {
    GLuint vertex_buffer = manager.MakeStaticDrawBuffer(vertices);

    GLuint uv_buffer = manager.MakeStaticDrawBuffer(uvs);

    GLuint normal_buffer = manager.MakeStaticDrawBuffer(normals);

    // 1-st attribute buffer - vertices.
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    // 2-nd attribute buffer - UVs.
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    // 3-rd attribute buffer - normals.
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
  }

 private:
  glm::vec3 cur_look_at_{1.0f, 0.0f, 1.0f};
  std::vector<Bone> bones_;
  std::vector<Doggo> doggos_;
  std::vector<Heart> hearts_;
  std::optional<Ground> ground_;
  std::optional<Sky> sky_;
  std::optional<Light> light_;

  int64_t bones_count_ = 0;
  int64_t doggo_fed_ = 0;
  int64_t doggos_count_ = 0;
  int64_t days_count_ = 0;
  float speed_slowdown_ = 1.0f;


  std::vector<glm::vec3> doggos_vertices_;
  std::vector<glm::vec3> doggos_normals_;
  std::vector<glm::vec2> doggos_uvs_;

  std::vector<glm::vec3> bones_vertices_;
  std::vector<glm::vec3> bones_normals_;
  std::vector<glm::vec2> bones_uvs_;

  // To make some delay for shooting and spawning new doggos.
  double last_bone_time_ = 0;
  double last_doggo_time_ = 0;
};