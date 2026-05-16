//----------------------------------------------------------------------------------------
/**
 * \file    Camera.cpp
 * \brief   Implementation of the Camera class handling movement and viewing modes.
 * \author  Laura Katerina Dudkova
 * \date    2026/05/11
 */

#include "Camera.h"

 /**
  * \brief Initializes the camera with default vectors, predefined positions, and starting angles.
  */
Camera::Camera()
{
    position = startCameraPosition;
    direction = StartCameraDirection;
    upVector = startCameraUpVector;

    currMode = startCameraMode;

    pitch = startCameraPitch;
    yaw = startCameraYaw;
    speed = cameraSpeed;
    colliderRadius = cameraColliderRadius;
}

/**
 * \brief Moves the camera in the specified direction.
 * \param dir The direction vector indicating where to move.
 */
void Camera::Move(glm::vec3 dir) {
    if (currMode == FREE_LOOK) {
        position += normalize(dir) * speed;
    }
}

/**
 * \brief Toggles through the available camera modes and updates vectors accordingly.
 * \details Upon switching to a static position (POSITION_1 or POSITION_2), it forces the
 *          camera to look at a specific angle using preset yaw and pitch, and recalculates
 *          the directional vector using trigonometry.
 */
void Camera::switchMode() {
    currMode = (currMode + 1) % MODES_NUM;

    //set camera position
    if (currMode == POSITION_1) {
        position = cameraPosition1;
        yaw = 50.0f;
        pitch = -20.0f;
    }
    else if (currMode == POSITION_2) {
        position = cameraPosition2;
        yaw = 120.0f;
        pitch = -25.0f;
    }

    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction = glm::normalize(direction);

    //switching to free look does not restart position, camera stays in the smae place in the same place
}