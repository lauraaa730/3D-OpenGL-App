//----------------------------------------------------------------------------------------
/**
 * \file    Camera.h
 * \brief   Header file for the Camera class and viewing modes.
 * \author  Laura Katerina Dudkova
 * \date    2026/05/11
 */

#pragma once
#include "pgr.h"
#include <iostream>

 /**
  * \enum Mode
  * \brief Defines the available viewing modes for the camera.
  */
enum Mode
{
	POSITION_1, ///< Static preset view position 1.
	POSITION_2, ///< Static preset view position 2.
	FREE_LOOK,  ///< Free-fly mode controlled by mouse and keyboard.
	FISH_VIEW,
	MODES_NUM,  ///< Total number of camera modes (used for looping).

};

/**
* \brief Default values for camera parameters.
*/
const float cameraSpeed = 0.05;
const glm::vec3 startCameraPosition = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 StartCameraDirection = glm::vec3(0.0f, 0.0f, -1.0f);
const glm::vec3 startCameraUpVector = glm::vec3(0.0f, 1.0f, 0.0f);

const Mode startCameraMode = FREE_LOOK;
const glm::vec3 cameraPosition1 = glm::vec3(-3.0, 3.0, -4.0);
const glm::vec3 cameraPosition2 = glm::vec3(5.0, 5.0, -5.0);

const float startCameraPitch = 0.0f;
const float startCameraYaw = -90.0f;
const float cameraColliderRadius = 1.0f;

/**
 * \class Camera
 * \brief Manages the 3D viewing camera, its modes, position, and orientation calculations.
 */
class Camera
{
public:
	/**
	 * \brief Default constructor. Initializes the camera vectors, default mode, and preset positions.
	 */
	Camera();

	float speed;            ///< \brief Movement speed multiplier for free-look mode.
	int currMode;           ///< \brief Current active viewing mode (from the Mode enum).
	float colliderRadius;   ///< \brief Radius used for checking collisions with objects.

	glm::vec3 position;     ///< \brief Current 3D world position of the camera.
	glm::vec3 direction;    ///< \brief Normalized vector pointing in the direction the camera is looking.
	glm::vec3 upVector;     ///< \brief Normalized vector pointing 'up' relative to the camera.

	float yaw;				///< \brief Camera yaw angle in degrees (elevation).
	float pitch;			///< \brief Camera pitch angle in degrees (rotation around y axis).

	/**
	 * \brief Moves the camera by translating its position along the given direction.
	 * \details Movement is only applied if the camera is currently in FREE_LOOK mode.
	 * \param dir The normalized direction vector to move along.
	 */
	void Move(glm::vec3 dir);

	/**
	 * \brief Cycles the camera to the next viewing mode.
	 * \details Switches between FREE_LOOK, POSITION_1, and POSITION_2. Recalculates
	 *          yaw, pitch, and the direction vector when snapping to a preset position.
	 */
	void switchMode();

};