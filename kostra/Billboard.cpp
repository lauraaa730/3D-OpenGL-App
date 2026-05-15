//----------------------------------------------------------------------------------------
/**
 * \file    Billboard.cpp
 * \author  Laura Katerina Dudkova
 * \date    2026/05/11
 * \brief   Implementation of the Billboard class for animated 2D sprites and additive blending.
 */

#include "Billboard.h"

Billboard::Billboard(const std::string& texturePath, ShaderProgram* shdrPrg)
	: ObjectInstance(shdrPrg) {

	//allocate the geometry and material structs
	geometry = new ObjectGeometry();
	geometry->numTriangles = 2; //1 quad
	geometry->elementBufferObject = 0;

	material = new ObjectMaterial();
	material->ambient = glm::vec3(1.0f);
	material->diffuse = glm::vec3(1.0f);
	material->specular = glm::vec3(0.0f);
	material->shininess = 1.0f;

	//x, y, z, u, v (normals removed)
	float quadVertices[] = {
		 0.5f,  0.5f, 0.0f,   /*0.0f, 0.0f, 1.0f,*/   1.0f, 1.0f,
		 0.5f, -0.5f, 0.0f,   /*0.0f, 0.0f, 1.0f,*/   1.0f, 0.0f,
		-0.5f,  0.5f, 0.0f,   /*0.0f, 0.0f, 1.0f,*/   0.0f, 1.0f,

		 0.5f, -0.5f, 0.0f,   /*0.0f, 0.0f, 1.0f,*/   1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,   /*0.0f, 0.0f, 1.0f,*/   0.0f, 0.0f,
		-0.5f,  0.5f, 0.0f,   /*0.0f, 0.0f, 1.0f,*/   0.0f, 1.0f
	};

	//vao
	glGenVertexArrays(1, &geometry->vertexArrayObject);
	glBindVertexArray(geometry->vertexArrayObject);

	//vbo
	glGenBuffers(1, &geometry->vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

	//position attribute
	glEnableVertexAttribArray(shaderProgram->locations.position);
	glVertexAttribPointer(shaderProgram->locations.position,
		3, //xyz
		GL_FLOAT, GL_FALSE,
		5 * sizeof(float), //vertex every 5 floats (xyz + uv) - stride
		(void*)0
	);

	//texture coordinate attribute
	glEnableVertexAttribArray(shaderProgram->locations.texCoord);
	glVertexAttribPointer(shaderProgram->locations.texCoord,
		2,
		GL_FLOAT, GL_FALSE,
		5 * sizeof(float), //stride
		(void*)(3 * sizeof(float)) //float
	);

	//unbind vao
	glBindVertexArray(0);

	//load texture
	geometry->texture = pgr::createTexture(texturePath);

	//initialize values
	currentFrame = 0;
	totalFrames = 16;
	timePerFrame = 1.0f / 10.0f; //fps
}

Billboard::~Billboard() {
	if (geometry != nullptr) {
		glDeleteVertexArrays(1, &geometry->vertexArrayObject);
		glDeleteBuffers(1, &geometry->vertexBufferObject);
		if (geometry->texture != 0) glDeleteTextures(1, &geometry->texture);
		delete geometry;
	}
	if (material != nullptr) delete material;
}

void Billboard::draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {
	if (geometry == nullptr || shaderProgram == nullptr || !shaderProgram->initialized) return;

	//enable additive blending and disable depth mask
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glDepthMask(GL_FALSE);

	glUseProgram(shaderProgram->program);

	//send the current frame to the shader
	GLint frameLoc = glGetUniformLocation(shaderProgram->program, "currentFrame");
	if (frameLoc != -1) {
		glUniform1i(frameLoc, currentFrame);
	}

	//calculate and send only the pvm matrix
	glm::mat4 pvmMatrix = projectionMatrix * viewMatrix * globalModelMatrix;
	glUniformMatrix4fv(shaderProgram->locations.PVMmatrix, 1, GL_FALSE, glm::value_ptr(pvmMatrix));

	//bind texture
	if (geometry->texture != 0) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, geometry->texture);
		glUniform1i(shaderProgram->locations.texSampler, 0);
	}

	//draw the hardcoded quad (6 vertices)
	glBindVertexArray(geometry->vertexArrayObject);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	//cleanup - restore default drawing states
	glDepthMask(GL_TRUE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_BLEND);

	//draw children
	for (ObjectInstance* child : children) {
		if (child != nullptr) {
			child->draw(viewMatrix, projectionMatrix);
		}
	}
}

void Billboard::update(const float elapsedTime, const glm::mat4* parentModelMatrix) {

	//update frame based on elapsed time
	currentFrame = (int)(elapsedTime / timePerFrame) % totalFrames;

	//call the base class update so matrices still calculate
	ObjectInstance::update(elapsedTime, parentModelMatrix);
}