#pragma once

#include "pgr.h"
#include "Spline.h"
#include <iostream>

/**
 * \brief Shader program related stuff (id, locations, ...).
 */
typedef struct _ShaderProgram {
	/// identifier for the shader program
	GLuint program;

	bool initialized;

	/**
	  * \brief Indices of the vertex shader inputs (locations)
	  */
	struct {
		// vertex attributes locations
		GLint position;
		GLint color;
		GLint texCoord;

		// uniforms locations
		GLint PVMmatrix;
		GLint texSampler;
		GLint hasTexture;
		
	} locations;

	// ...

	_ShaderProgram() : program(0), initialized(false) {
		locations.position = -1;
		locations.PVMmatrix = -1;
		locations.color = -1;
		locations.texCoord = -1;
		//TODO zbytek priradit -1
	}

} ShaderProgram;

/**
 * \brief Geometry of an object (vertices, triangles).
 */
typedef struct _ObjectGeometry {
	GLuint        vertexBufferObject;   ///< identifier for the vertex buffer object
	GLuint        elementBufferObject;  ///< identifier for the element buffer object
	GLuint        vertexArrayObject;    ///< identifier for the vertex array object
	unsigned int  numTriangles;         ///< number of triangles in the mesh
	GLuint        texture;

} ObjectGeometry;

class ObjectInstance;
/**
 * \brief Linear representation of the scene objects.  The objects themselves may represent the subtrees.
 */
typedef std::vector<ObjectInstance*> ObjectList;  

class ObjectInstance {

protected:

	ObjectGeometry* geometry;
	glm::mat4		localModelMatrix;
	glm::mat4		globalModelMatrix;

	glm::vec3		positionInWorld;
	glm::vec3		direction;
	glm::vec3		upVector;
	float			scale;

	// dynamic objects
	bool			isDynamic;
	float			speed;
	glm::vec3		startPosition;
	

	ShaderProgram* shaderProgram;

	ObjectList children;

public:

	/**
	 * \brief ObjectInstance constructor. Takes a pointer to the shader and must create object resources (VBO and VAO)
	 * \param shdrPrg pointer to the shader program for rendering objects data
	 */
	/* puvodni konstruktor
	ObjectInstance(ShaderProgram* shdrPrg = nullptr) : geometry(nullptr), shaderProgram(shdrPrg) {}
	~ObjectInstance() {}
	*/

	ObjectInstance(ShaderProgram* shdrPrg = nullptr) {
		geometry = nullptr;
		shaderProgram = shdrPrg;
		positionInWorld = glm::vec3(0.0f, 0.0f, 0.0f);
		isDynamic = false;
		speed = 0.5f; 
		upVector = glm::vec3(0.0f, 1.0f, 0.0f);
		direction = glm::vec3(0.0f, 0.0f, 1.0f);
		startPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	}
  
	/**
	* \brief Recalculates the global matrix and updates all children.
	*   Derived classes should also call this method (using ObjectInstance::update()).
	* \param elapsedTime time value in seconds, such as 0.001*glutGet(GLUT_ELAPSED_TIME) (conversion milliseconds => seconds)
	* \param parentModelMatrix parent transformation in the scene-graph subtree
	*/
	virtual void update(const float elapsedTime, const glm::mat4* parentModelMatrix) {
		//update animation in dynamic objects
		if (isDynamic) {
			//std::cout << positionInWorld.x << positionInWorld.y << std::endl;
			float splineTime = elapsedTime * speed;

			glm::vec3 pos = findPointOnClosedCurve(splineTime);
			glm::vec3 dir = find_1stDerivative_OnClosedCurve(splineTime);

			setPosition(startPosition + pos);
			setDirection(dir);

			transformObject();
		}
		
		// if we have parent, multiply parent's matrix with ours
		if (parentModelMatrix != nullptr)
			globalModelMatrix = *parentModelMatrix * localModelMatrix;
		else
			globalModelMatrix = localModelMatrix;

		


		// update all children
		for (ObjectInstance* child : children) {
			if (child != nullptr)
				child->update(elapsedTime, &globalModelMatrix);
		}
	}

	/**
	 * \brief Draw instance geometry and calls the draw() on child nodes.
	 */
	virtual void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {
		// draw instance geometry using globalModelMatrix
		// ...

		// process all children
		for (ObjectInstance* child : children) {   //for (auto child : children) {
			if (child != nullptr)
				child->draw(viewMatrix, projectionMatrix);
		}
	}

	virtual void transformObject() {
		glm::vec3 x = -glm::normalize(direction);

		glm::vec3 z = -glm::normalize(glm::cross(upVector, x));

		glm::vec3 y = glm::cross(z, x);

		x = x * scale;
		y = y * scale;
		z = z * scale;

		glm::mat4 matrix = glm::mat4(
			x.x, x.y, x.z, 0.0,
			y.x, y.y, y.z, 0.0,
			z.x, z.y, z.z, 0.0,
			positionInWorld.x, positionInWorld.y, positionInWorld.z, 1.0
		);

		localModelMatrix = matrix;
	}

	virtual void setScale(float sc) {
		scale = sc;
	}

	virtual void setPosition(glm::vec3 new_position) {
		//TODO jaky je presne rozdil mezi globalModelMatrix a localModelMatrix?
		positionInWorld = new_position;
	}

	virtual void setIsDynamic(bool parameter) {
		isDynamic = parameter;
	}

	virtual void setDirection(glm::vec3 dir) {
		direction = dir;
	}

	virtual void setUpVector(glm::vec3 upVec) {
		upVector = upVec;
	}

	virtual void setStartPosition(glm::vec3 pos) {
		startPosition = pos;
		positionInWorld = pos;
	}

	
};
