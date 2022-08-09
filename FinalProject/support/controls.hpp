#pragma once

#include <glm/glm.hpp>

namespace support {

glm::vec3 GetPosition();
void ComputeMatricesFromInputs(GLFWwindow* window);
glm::mat4 GetViewMatrix();
glm::mat4 GetProjectionMatrix();

void IncreaseMovementSpeedCoefBy(float factor);
void DecreaseMovementSpeedCoefBy(float factor);
}  // namespace support
