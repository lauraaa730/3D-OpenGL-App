#include "Camera.h"

Camera::Camera()
{
    position = glm::vec3(0.0f, 0.0f, 3.0f);
    direction = glm::vec3(0.0f, 0.0f, -1.0f);
    upVector = glm::vec3(0.0f, 1.0f, 0.0f);
}

void Camera::Move(glm::vec3 dir) {
    position += dir;
}
