//----------------------------------------------------------------------------------------
/**
 * \file    HardcodedObject.h
 * \author  Laura Katerina Dudkova
 * \date    2026/05/11
 * \brief   Header file for the HardcodedObject class representing smooth-shaded, indexed geometry.
 */

#pragma once

#include "object.h"
#include "SceneObjectsData.h"

/**
 * \class HardcodedObject
 * \brief Object with hard-coded vertices and colors using VBO, VAO, and EBO for smooth shading.
 */
class HardcodedObject : public ObjectInstance
{
public:

	/**
	 * \brief Constructor initializing geometry and material from hardcoded data.
	 * \param shdrPrg Pointer to the shader program.
	 * \param model Pointer to the hardcoded model data structure.
	 */
	HardcodedObject(ShaderProgram* shdrPrg = nullptr, const HardCodedModel* model = nullptr);
	~HardcodedObject();

	//void update(float elapsedTime, const glm::mat4* parentModelMatrix) override;
	
	/**
	 * \brief Draws the hardcoded object using indexed rendering.
	 * \param viewMatrix The active camera view matrix.
	 * \param projectionMatrix The active camera projection matrix.
	 */
	void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) override;

private:

	bool initialized;  ///< \brief true if object has the shader with defined locations
};