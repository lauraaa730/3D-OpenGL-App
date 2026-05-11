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
	FREE_LOOK,  ///< Free-fly mode controlled by mouse and keyboard.
	POSITION_1, ///< Static preset view position 1.
	POSITION_2, ///< Static preset view position 2.
	MODES_NUM,  ///< Total number of camera modes (used for looping).

};

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

	glm::vec3 position1;    ///< \brief Hardcoded 3D coordinates for static POSITION_1.
	glm::vec3 position2;    ///< \brief Hardcoded 3D coordinates for static POSITION_2.

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