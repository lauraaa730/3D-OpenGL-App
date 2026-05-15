//----------------------------------------------------------------------------------------
/**
 * \file    Billboard.h
 * \author  Laura Katerina Dudkova
 * \date    2026/05/11
 * \brief   Header file for the Billboard class, representing an animated 2D sprite.
 */

#pragma once

#include "object.h"
#include "pgr.h"
#include <string>

 /**
  * \class Billboard
  * \brief A 2D textured quad that can handle sprite sheet animations.
  */
class Billboard : public ObjectInstance {
public:
	int currentFrame; ///< \brief current animation frame index
	float timePerFrame; ///< \brief duration of each frame in seconds
	int totalFrames; ///< \brief total number of frames in the sprite sheet

	/**
	 * \brief Constructor initializing the hardcoded quad and material.
	 * \param texturePath Path to the sprite sheet texture.
	 * \param shdrPrg Pointer to the billboard shader program.
	 */
	Billboard(const std::string& texturePath, ShaderProgram* shdrPrg);

	~Billboard();

	/**
	 * \brief Draws the billboard using additive blending.
	 * \param viewMatrix The active camera view matrix.
	 * \param projectionMatrix The active camera projection matrix.
	 */
	virtual void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) override;

	/**
	 * \brief Updates the current animation frame based on elapsed time.
	 */
	virtual void update(const float elapsedTime, const glm::mat4* parentModelMatrix) override;
};