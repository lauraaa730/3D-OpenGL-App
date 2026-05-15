//----------------------------------------------------------------------------------------
/**
 * \file    HardcodedObject.cpp
 * \author  Laura Katerina Dudkova
 * \date    2026/05/11
 * \brief   Implementation of the HardcodedObject class, handling normal generation and indexed rendering.
 */

#include "HardcodedObject.h"
#include <iostream>


 /*void HardcodedObject::update(float elapsedTime, const glm::mat4* parentModelMatrix) {
	 //propagate the update to children
	 ObjectInstance::update(elapsedTime, parentModelMatrix);
 }*/

void HardcodedObject::draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
	if (initialized && (shaderProgram != nullptr)) {

		//enable additive blending
		glEnable(GL_BLEND);

		glUseProgram(shaderProgram->program);

		glm::mat4 PVM = projectionMatrix * viewMatrix * globalModelMatrix;

		//send matrix to shader uniform
		glUniformMatrix4fv(shaderProgram->locations.PVMmatrix, 1, GL_FALSE,
			glm::value_ptr(PVM));

		glUniformMatrix4fv(shaderProgram->locations.Mmatrix, 1, GL_FALSE, glm::value_ptr(globalModelMatrix));

		//send material properties
		glUniform3fv(shaderProgram->locations.matDiffuse, 1, glm::value_ptr(material->diffuse));
		glUniform3fv(shaderProgram->locations.matAmbient, 1, glm::value_ptr(material->ambient));
		glUniform3fv(shaderProgram->locations.matSpecular, 1, glm::value_ptr(material->specular));
		glUniform1f(shaderProgram->locations.matShininess, material->shininess);

		glm::mat4 normalMat4 = glm::transpose(glm::inverse(globalModelMatrix));
		glm::mat3 normalMat3 = glm::mat3(normalMat4);
		glUniformMatrix3fv(shaderProgram->locations.normalMatrix, 1, GL_FALSE, glm::value_ptr(normalMat3));


		//no texture
		glUniform1i(shaderProgram->locations.hasTexture, 0);

		//alpha
		glUniform1f(shaderProgram->locations.alpha, alpha);

		//bind vao for this specific object
		glBindVertexArray(geometry->vertexArrayObject);

		//draw using indices
		glDrawElements(
			GL_TRIANGLES,                    //draw triangles
			geometry->numTriangles * 3,      //total indices
			GL_UNSIGNED_INT,                 //type of indices[]
			0                                //start at beginning
		);

		//unbind vao
		glBindVertexArray(0);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_BLEND);

	}
	else {
		std::cerr << "HardcodedObject::draw(): Can't draw, HardcodedObject not initialized properly!" << std::endl;
	}

	//maybe draw children too?
	//ObjectInstance::draw(viewMatrix, projectionMatrix);
}

HardcodedObject::HardcodedObject(ShaderProgram* shdrPrg, const HardCodedModel* model)
	: ObjectInstance(shdrPrg)
{
	geometry = new ObjectGeometry;
	material = new ObjectMaterial;

	material->ambient = model->ambient;
	material->diffuse = model->diffuse;
	material->specular = model->specular;
	material->shininess = model->shininess;

	setAlpha(model->alpha);
	setStartPosition(model->position);
	setIsDynamic(model->isDynamic);
	setDirection(model->direction);
	setScale(model->scale);


	//object data
	const float* vertices = model->vertices;
	const unsigned int* indices = model->indeces;

	geometry->numTriangles = model->trianglesNum;

	int numIndices = model->trianglesNum * 3;

	//calculate normals ---
	std::vector<glm::vec3> calculatedNormals(model->verticesNum, glm::vec3(0.0f));

	for (int i = 0; i < numIndices; i += 3) {
		unsigned int idx0 = indices[i];
		unsigned int idx1 = indices[i + 1];
		unsigned int idx2 = indices[i + 2];

		//extract 3d positions of the triangle's 3 vertices
		//6 floats per vertex: x,y,z,r,g,b
		glm::vec3 v0(vertices[idx0 * 6], vertices[idx0 * 6 + 1], vertices[idx0 * 6 + 2]);
		glm::vec3 v1(vertices[idx1 * 6], vertices[idx1 * 6 + 1], vertices[idx1 * 6 + 2]);
		glm::vec3 v2(vertices[idx2 * 6], vertices[idx2 * 6 + 1], vertices[idx2 * 6 + 2]);

		//calculate two edges of the triangle
		glm::vec3 edge1 = v1 - v0;
		glm::vec3 edge2 = v2 - v0;

		//calculate normal of this specific face
		glm::vec3 faceNormal = glm::normalize(glm::cross(edge2, edge1));

		//add this face normal to all 3 vertices that touch this face
		calculatedNormals[idx0] += faceNormal;
		calculatedNormals[idx1] += faceNormal;
		calculatedNormals[idx2] += faceNormal;
	}

	//normalize all normals to smooth them out across faces
	for (int i = 0; i < model->verticesNum; ++i) {
		calculatedNormals[i] = glm::normalize(calculatedNormals[i]);
	}

	//vao
	glGenVertexArrays(1, &geometry->vertexArrayObject);
	glBindVertexArray(geometry->vertexArrayObject);

	//vbo for vertices
	glGenBuffers(1, &geometry->vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, model->verticesNum * 6 * sizeof(float), vertices, GL_STATIC_DRAW);

	//ebo for indeces 
	glGenBuffers(1, &geometry->elementBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->elementBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	if ((shaderProgram != nullptr) && shaderProgram->initialized && (shaderProgram->locations.position != -1) && (shaderProgram->locations.PVMmatrix != -1)) {

		//position attribute
		glEnableVertexAttribArray(shaderProgram->locations.position);
		glVertexAttribPointer(
			shaderProgram->locations.position,
			3, //x,y,z
			GL_FLOAT,
			GL_FALSE,  //do not normalize
			6 * sizeof(float), //full vertex size - coordinates + color
			(void*)0 //vertex position starts at offset 0 in each vertex
		);

		//color attribute
		glEnableVertexAttribArray(shaderProgram->locations.color);
		glVertexAttribPointer(
			shaderProgram->locations.color,
			3, //r,g,b
			GL_FLOAT,
			GL_FALSE,  //do not normalize
			6 * sizeof(float), //full vertex size - coordinates + color
			(void*)(3 * sizeof(float)) //after position
		);

		//normals attribute
		if (shaderProgram->locations.normal != -1) {
			glGenBuffers(1, &geometry->normalBufferObject);
			glBindBuffer(GL_ARRAY_BUFFER, geometry->normalBufferObject);

			glBufferData(GL_ARRAY_BUFFER, calculatedNormals.size() * sizeof(glm::vec3), calculatedNormals.data(), GL_STATIC_DRAW);

			glEnableVertexAttribArray(shaderProgram->locations.normal);
			glVertexAttribPointer(shaderProgram->locations.normal,
				3,
				GL_FLOAT,
				GL_FALSE,
				0,
				0
			);
		}

		initialized = true;
	}
	else {
		std::cerr << "HardcodedObject::HardcodedObject(): shaderProgram struct not initialized!" << std::endl;
	}

	glBindVertexArray(0);
}

HardcodedObject::~HardcodedObject() {
	glDeleteVertexArrays(1, &(geometry->vertexArrayObject));
	glDeleteBuffers(1, &(geometry->elementBufferObject));
	glDeleteBuffers(1, &(geometry->vertexBufferObject));
	glDeleteBuffers(1, &(geometry->normalBufferObject));

	delete geometry;
	geometry = nullptr;

	initialized = false;
}