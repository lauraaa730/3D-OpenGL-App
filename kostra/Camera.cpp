#include "Camera.h"

Camera::Camera()
{
    position = glm::vec3(0.0f, 1.0f, 0.0f);
    direction = glm::vec3(0.0f, 0.0f, -1.0f);
    upVector = glm::vec3(0.0f, 1.0f, 0.0f);

    currMode = FREE_LOOK;
    position1 = glm::vec3(-3.0, 3.0, -4.0);
    position2 = glm::vec3(5.0, 5.0, -5.0);

    pitch = 0.0f;
    yaw = -90.0f;
    speed = 0.1f;
    colliderRadius = 1.0f;

    //container = new SphereCollider;
    //container->center = p
}

void Camera::Move(glm::vec3 dir) {
    if (currMode == FREE_LOOK) {
        position += normalize(dir) * speed;
    }
}

void Camera::switchMode() {
    currMode = (currMode+1)%MODES_NUM; 

    //set camera position
    if (currMode == POSITION_1) {
        position = position1;
        //TODO make the yaw and pitch not these random numbers, but maybe -position or smth
        yaw = 50.0f;
        pitch = -20.0f;
    } else if (currMode == POSITION_2) {
        position = position2;
        yaw = 120.0f;
        pitch = -25.0f;
    }

    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction = glm::normalize(direction);

    //if we switch to free look, we dont restart our position
}
