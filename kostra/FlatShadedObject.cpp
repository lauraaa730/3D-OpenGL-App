//----------------------------------------------------------------------------------------
/**
 * \file    FlatShadedObject.cpp
 * \author  Laura Katerina Dudkova
 * \date    2026/05/11
 * \brief   Implementation of the FlatShadedObject class, handling face normal calculations.
 */

#include "FlatShadedObject.h"
#include <iostream>


 /*void FlatShadedObject::update(float elapsedTime, const glm::mat4* parentModelMatrix) {
	 //propagate the update to children
	 ObjectInstance::update(elapsedTime, parentModelMatrix);
 }*/

void FlatShadedObject::draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
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

		//draw vbo
		glDrawArrays(
			GL_TRIANGLES,                   //draw triangles
			0,                              //start at beginning
			geometry->numTriangles * 3      //total number of vertices
		);

		//unbind vao
		glBindVertexArray(0);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_BLEND);

	}
	else {
		std::cerr << "FlatShadedObject::draw(): Can't draw, FlatShadedObject not initialized properly!" << std::endl;
	}

	//maybe draw children too?
	//ObjectInstance::draw(viewMatrix, projectionMatrix);
}

FlatShadedObject::FlatShadedObject(ShaderProgram* shdrPrg, const FlatShadedModel* model)
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

	geometry->numTriangles = model->trianglesNum;



	//create new unrolled vectors for vertices and flat normals
	std::vector<float> flatVertices;
	std::vector<glm::vec3> flatNormals;

	//each triangle consists of 3 vertices, which is 9 floats (3 * 3) in the array
	for (int i = 0; i < geometry->numTriangles; i++) {

		int offset = i * 9; //starting index in the float array for this triangle

		//extract 3d positions of the triangle's 3 vertices
		//read directly from the linear array without using indices
		glm::vec3 v0(vertices[offset + 0], vertices[offset + 1], vertices[offset + 2]);
		glm::vec3 v1(vertices[offset + 3], vertices[offset + 4], vertices[offset + 5]);
		glm::vec3 v2(vertices[offset + 6], vertices[offset + 7], vertices[offset + 8]);

		//calculate two edges of the triangle
		glm::vec3 edge1 = v1 - v0;
		glm::vec3 edge2 = v2 - v0;

		//calculate normal of this specific face
		glm::vec3 faceNormal = glm::normalize(glm::cross(edge2, edge1));

		//push data for all 3 vertices of this triangle
		for (int j = 0; j < 3; ++j) {
			int vOffset = offset + (j * 3);

			//add position (x, y, z)
			flatVertices.push_back(vertices[vOffset + 0]);
			flatVertices.push_back(vertices[vOffset + 1]);
			flatVertices.push_back(vertices[vOffset + 2]);

			//add color (r, g, b) from the single model color
			flatVertices.push_back(1.0f);
			flatVertices.push_back(1.0f);
			flatVertices.push_back(1.0f);

			//add the exact same face normal to all three vertices
			flatNormals.push_back(faceNormal);
		}
	}

	//vao
	glGenVertexArrays(1, &geometry->vertexArrayObject);
	glBindVertexArray(geometry->vertexArrayObject);

	//vbo - bind the unrolled flatvertices instead of raw model vertices
	glGenBuffers(1, &geometry->vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, flatVertices.size() * sizeof(float), flatVertices.data(), GL_STATIC_DRAW);


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

			glBufferData(GL_ARRAY_BUFFER, flatNormals.size() * sizeof(glm::vec3), flatNormals.data(), GL_STATIC_DRAW);

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
		std::cerr << "FlatShadedObject::FlatShadedObject(): shaderProgram struct not initialized!" << std::endl;
	}

	glBindVertexArray(0);
}

FlatShadedObject::~FlatShadedObject() {
	glDeleteVertexArrays(1, &(geometry->vertexArrayObject));
	glDeleteBuffers(1, &(geometry->vertexBufferObject));
	glDeleteBuffers(1, &(geometry->normalBufferObject));

	delete geometry;
	geometry = nullptr;

	initialized = false;
}