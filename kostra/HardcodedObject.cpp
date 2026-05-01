#include "HardcodedObject.h"
#include <iostream>


void HardcodedObject::update(float elapsedTime, const glm::mat4* parentModelMatrix) {
	// propagate the update to children
	ObjectInstance::update(elapsedTime, parentModelMatrix);
}

void HardcodedObject::draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
	if (initialized && (shaderProgram != nullptr)) {
		glUseProgram(shaderProgram->program);

		glm::mat4 PVM = projectionMatrix * viewMatrix * globalModelMatrix;

		//send matrix to shader uniform
		glUniformMatrix4fv(shaderProgram->locations.PVMmatrix, 1, GL_FALSE,
			glm::value_ptr(PVM));

		//no texture
		glUniform1i(shaderProgram->locations.hasTexture, 0);

		//bind VAO for this specific object
		glBindVertexArray(geometry->vertexArrayObject);

		//draw using indices
		glDrawElements(
			GL_TRIANGLES,                    // draw triangles
			geometry->numTriangles * 3,     // total indices
			GL_UNSIGNED_INT,                // type of indices[]
			0                               // start at beginning
		);

		//unbind VAO
		glBindVertexArray(0);

	}
	else {
		std::cerr << "HardcodedObject::draw(): Can't draw, HardcodedObject not initialized properly!" << std::endl;
	}

	//maybe draw children too?
	//ObjectInstance::draw(viewMatrix, projectionMatrix);
}

HardcodedObject::HardcodedObject(ShaderProgram* shdrPrg) : ObjectInstance(shdrPrg), initialized(false)
{
	geometry = new ObjectGeometry;

	//OBJECT DATA
	//TODO - MOVE TO DATA FILE AND READ FROM THERE
	static constexpr float vertices[] = {
		// back face
		-0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f, // 0 red
		 0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f, // 1 green
		 0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 1.0f, // 2 blue
		-0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f, // 3 yellow

		// front face
		-0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 1.0f, // 4 magenta
		 0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 1.0f, // 5 cyan
		 0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f, // 6 white
		-0.5f,  0.5f,  0.5f,   0.3f, 0.3f, 0.3f  // 7 gray
	};

	static constexpr unsigned int indices[] = {
		0,1,2,  2,3,0, // back
		4,5,6,  6,7,4, // front
		0,3,7,  7,4,0, // left
		1,5,6,  6,2,1, // right
		0,1,5,  5,4,0, // bottom
		3,2,6,  6,7,3 // top
	};

	geometry->numTriangles = 12;

	//VAO
	glGenVertexArrays(1, &geometry->vertexArrayObject);
	glBindVertexArray(geometry->vertexArrayObject);

	//VBO
	glGenBuffers(1, &geometry->vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//EBO
	glGenBuffers(1, &geometry->elementBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->elementBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	if ((shaderProgram != nullptr) && shaderProgram->initialized && (shaderProgram->locations.position != -1) && (shaderProgram->locations.PVMmatrix != -1)) {
		
		//position attribute
		glEnableVertexAttribArray(shaderProgram->locations.position);
		glVertexAttribPointer(
			shaderProgram->locations.position, 
			3, //x,y,z
			GL_FLOAT, 
			GL_FALSE,  //do not normalize
			6* sizeof(float), //full vertex size - coordinates + color
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
			(void*)(3*sizeof(float)) //after position
		); 

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

	delete geometry;
	geometry = nullptr;

	initialized = false;
}

