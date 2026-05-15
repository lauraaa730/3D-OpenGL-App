//----------------------------------------------------------------------------------------
/**
 * \file    singlemesh.h
 * \author  Laura Katerina Dudkova
 * \date    2026/05/11
 * \brief   Header file for the SingleMesh class, representing 3D models with textures.
 */

#pragma once

#include "object.h"

 /**
  * \class SingleMesh
  * \brief Class representing a 3D object loaded via Assimp that requires a texture.
  * \details Inherits from ObjectInstance and handles loading, aggregating, and drawing mesh data.
  */
class SingleMesh : public ObjectInstance
{
public:

	SingleMesh(const std::string& fileName, const std::string& textureName, ShaderProgram* shdrPrg = nullptr);
	~SingleMesh();

	void update(float elapsedTime, const glm::mat4* parentModelMatrix) override;
	void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) override;

private:

	/**
	 * \brief Loads geometry and material data from an .obj file using the Assimp library.
	 * \param fileName File path to the 3D model.
	 * \param shader Pointer to the shader program struct to link attributes.
	 * \param geometry Pointer to the pointer where geometry data will be stored.
	 * \param material Pointer to the pointer where material data will be stored.
	 * \return True if the mesh and VAO successfully initialized, false otherwise.
	 */
	bool loadSingleMesh(const std::string& fileName, ShaderProgram* shader, ObjectGeometry** geometry, ObjectMaterial** material);

	bool initialized;  ///< \brief flag indicating if the object and its shader locations are ready.
	GLuint textureID;  ///< \brief OpenGL texture identifier for this mesh.
};