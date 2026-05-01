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
		// -------- top ring --------
	 0.00f,  0.90f,  0.00f,   0.70f, 0.70f, 0.70f,
	 0.35f,  0.78f,  0.18f,   0.62f, 0.62f, 0.62f,
	 0.18f,  0.80f,  0.42f,   0.68f, 0.68f, 0.68f,
	-0.20f,  0.76f,  0.38f,   0.58f, 0.58f, 0.58f,
	-0.42f,  0.74f,  0.10f,   0.65f, 0.65f, 0.65f,
	-0.34f,  0.77f, -0.22f,   0.60f, 0.60f, 0.60f,
	-0.08f,  0.81f, -0.40f,   0.72f, 0.72f, 0.72f,
	 0.26f,  0.79f, -0.34f,   0.57f, 0.57f, 0.57f,
	 0.44f,  0.75f, -0.06f,   0.66f, 0.66f, 0.66f,

	 // -------- middle ring --------
	  0.62f,  0.35f,  0.10f,   0.52f, 0.52f, 0.52f,
	  0.44f,  0.30f,  0.48f,   0.60f, 0.60f, 0.60f,
	  0.10f,  0.28f,  0.62f,   0.48f, 0.48f, 0.48f,
	 -0.32f,  0.33f,  0.56f,   0.64f, 0.64f, 0.64f,
	 -0.58f,  0.31f,  0.20f,   0.55f, 0.55f, 0.55f,
	 -0.55f,  0.27f, -0.22f,   0.46f, 0.46f, 0.46f,
	 -0.24f,  0.29f, -0.56f,   0.63f, 0.63f, 0.63f,
	  0.18f,  0.32f, -0.60f,   0.50f, 0.50f, 0.50f,
	  0.56f,  0.30f, -0.30f,   0.58f, 0.58f, 0.58f,

	  // -------- lower ring --------
	   0.52f, -0.12f,  0.14f,   0.42f, 0.42f, 0.42f,
	   0.34f, -0.16f,  0.44f,   0.49f, 0.49f, 0.49f,
	   0.02f, -0.20f,  0.52f,   0.38f, 0.38f, 0.38f,
	  -0.30f, -0.14f,  0.42f,   0.55f, 0.55f, 0.55f,
	  -0.50f, -0.18f,  0.08f,   0.41f, 0.41f, 0.41f,
	  -0.42f, -0.22f, -0.26f,   0.47f, 0.47f, 0.47f,
	  -0.14f, -0.18f, -0.46f,   0.36f, 0.36f, 0.36f,
	   0.24f, -0.16f, -0.48f,   0.51f, 0.51f, 0.51f,
	   0.48f, -0.14f, -0.18f,   0.44f, 0.44f, 0.44f,

	   // -------- bottom --------
		0.00f, -0.62f,  0.00f,   0.30f, 0.30f, 0.30f,
	};

	static constexpr unsigned int indices[] = {
		// top cap
	0,1,2,   0,2,3,   0,3,4,
	0,4,5,   0,5,6,   0,6,7,
	0,7,8,   0,8,1,

	// top ring -> middle ring
	1,9,10,   1,10,2,
	2,10,11,  2,11,3,
	3,11,12,  3,12,4,
	4,12,13,  4,13,5,
	5,13,14,  5,14,6,
	6,14,15,  6,15,7,
	7,15,16,  7,16,8,
	8,16,17,  8,17,1,
	1,17,9,

	// middle ring -> lower ring
	9,18,19,   9,19,10,
	10,19,20,  10,20,11,
	11,20,21,  11,21,12,
	12,21,22,  12,22,13,
	13,22,23,  13,23,14,
	14,23,24,  14,24,15,
	15,24,25,  15,25,16,
	16,25,26,  16,26,17,
	17,26,18,  17,18,9,

	// bottom taper
	18,27,19,
	19,27,20,
	20,27,21,
	21,27,22,
	22,27,23,
	23,27,24,
	24,27,25,
	25,27,26,
	26,27,18
	
	};

	geometry->numTriangles = 52;

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

