//----------------------------------------------------------------------------------------
/**
 * \file    FlatShadedObject.h
 * \author  Laura Katerina Dudkova
 * \date    2026/05/11
 * \brief   Header file for FlatShadedObject, a class representing flat-shaded geometry without EBOs.
 */

#pragma once

#include "object.h"
#include "SceneObjectsData.h"

 /**
  * \class FlatShadedObject
  * \brief Object with hard-coded vertices and colors using VBO and VAO for flat shading.
  * \details Unrolls triangles to calculate independent face normals, giving a faceted "low-poly" look.
  */
class FlatShadedObject : public ObjectInstance
{
public:

	/**
	 * \brief Constructor initializing geometry and material for flat shading.
	 * \param shdrPrg Pointer to the shader program.
	 * \param model Pointer to the hardcoded flat-shaded model data.
	 */
	FlatShadedObject(ShaderProgram* shdrPrg = nullptr, const FlatShadedModel* model = nullptr);
	~FlatShadedObject();

	//void update(float elapsedTime, const glm::mat4* parentModelMatrix) override;

	/**
	 * \brief Draws the flat-shaded object using glDrawArrays.
	 * \param viewMatrix The active camera view matrix.
	 * \param projectionMatrix The active camera projection matrix.
	 */
	void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) override;

private:

	bool initialized;  ///< \brief true if the shader locations are ready
};