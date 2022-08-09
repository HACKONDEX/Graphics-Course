#pragma once

#include <glm/glm.hpp>

class Light {
  enum State {
    kSunrise,
    kSunset,
    kMoonRise,
    kMoonSet,
  };

 public:
  Light(GLuint light_id, GLuint color_id, GLuint power_id)
      : light_id_(light_id), color_id_(color_id), power_id_(power_id) {
    SetToRisePosition(kSunrise);
    rise_direction_ = (zenith_position_ - rise_position_) / steps_count_;
    set_direction_ = (set_position_ - zenith_position_) / steps_count_;
  }

  bool UpdatePosition() {
    bool value = false;
    switch (light_state_) {
      case kSunrise: {
        std::vector<GLfloat> color = {0.976, 0.964, 0.643};
        GLfloat power = 150.f;
        if (InZenithRising()) {
          power = 300.f;
        } else {
          color = {0.949, 0.811, 0.227};
        }
        glUniform3fv(color_id_, 1, color.data());
        glUniform1f(power_id_, power);
        if (CanRise()) {
          position_ += rise_direction_ * speed_coefficient_;
        } else {
          light_state_ = kSunset;
        }
        return value;
      }
      case kSunset: {
        std::vector<GLfloat> color = {0.976, 0.964, 0.643};
        GLfloat power = 150.f;
        if (InZenithRising()) {
          power = 300.f;
        } else {
          color = {0.882, 0.243, 0.019};
        }
        glUniform3fv(color_id_, 1, color.data());
        glUniform1f(power_id_, power);
        if (CanSet()) {
          position_ += set_direction_ * speed_coefficient_;
        } else {
          SetToRisePosition(kMoonRise);
        }
        return value;
      }
      case kMoonRise: {
        std::vector<GLfloat> color = {0.235, 0.043, 0.678};
        GLfloat power = 125.f;
        glUniform3fv(color_id_, 1, color.data());
        glUniform1f(power_id_, power);
        if (CanRise()) {
          position_ += rise_direction_ * speed_coefficient_;
        } else {
          light_state_ = kMoonSet;
        }
        return value;
      }
      case kMoonSet: {
        std::vector<GLfloat> color = {0.066, 0.023, 0.396};
        GLfloat power = 125.f;
        glUniform3fv(color_id_, 1, color.data());
        glUniform1f(power_id_, power);
        if (CanSet()) {
          position_ += set_direction_ * speed_coefficient_;
        } else {
          SetToRisePosition(kSunrise);
          value = true;
        }
        return value;
      }
    }
  }

  void DecreaseSpeedBy(float factor) {
    speed_coefficient_ /= factor;
  }

  void IncreaseSpeedBy(float factor) {
    speed_coefficient_ *= factor;
  }

  void Display() {
    glUniform3f(light_id_, position_.x, position_.y, position_.z);
  }

 private:
  bool CanRise() {
    return position_[1] < zenith_position_[1];
  }

  bool CanSet() {
    return position_[1] > set_position_[1];
  }

  bool InZenithRising() {
    return position_.y > 17.f;
  }

  void SetToRisePosition(State state) {
    position_ = rise_position_;
    light_state_ = state;
  }

 private:
  GLuint light_id_;
  GLuint color_id_;
  GLuint power_id_;
  glm::vec3 position_;
  float speed_coefficient_ = 1.0f;
  State light_state_;
  glm::vec3 rise_direction_;
  glm::vec3 set_direction_;

  const glm::vec3 rise_position_{-32.2019f, -1.1368f, -1.15402f};
  const glm::vec3 zenith_position_{-0.408491f, 32.8551f, 3.27468f};
  const glm::vec3 set_position_{28.5246f, -0.79088f, -3.41769f};
  const float steps_count_ = 1000.0f;
};