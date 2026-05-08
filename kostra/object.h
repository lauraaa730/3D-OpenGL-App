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
		GLint normal;
		GLint color;
		GLint alpha;
		GLint texCoord;

		// uniforms locations
		GLint PVMmatrix;
		GLint texSampler;
		GLint hasTexture;
		GLint Mmatrix;
		GLint normalMatrix;
		GLint Vmatrix;
		GLint UVMatrix;

		GLint matAmbient;
		GLint matDiffuse;
		GLint matSpecular;
		GLint matShininess;

		//lights ---
		GLint moonLightAmbient;
		GLint moonLightDiffuse;
		GLint moonLightSpecular;
		GLint moonLightDirection;

		GLint fireflyAmbient;
		GLint fireflyDiffuse;
		GLint fireflySpecular;
		GLint fireflyPosition;
		GLint fireflyConstant;
		GLint fireflyLinear;
		GLint fireflyQuadratic;

		GLint lamp1Ambient;
		GLint lamp1Diffuse;
		GLint lamp1Specular;
		GLint lamp1Position;
		GLint lamp1Direction;
		GLint lamp1Constant;
		GLint lamp1Linear;
		GLint lamp1Quadratic;
		GLint lamp1SpotCosCutOff;
		GLint lamp1SpotExponent;

		GLint lanternPosition;
		GLint lanternAmbient;
		GLint lanternDiffuse;
		GLint lanternSpecular;
		GLint lanternConstant;
		GLint lanternLinear;
		GLint lanternQuadratic;
		
	} locations;

	// ...

	_ShaderProgram() : program(0), initialized(false) {
		locations.position = -1;
		locations.normal = -1;
		locations.PVMmatrix = -1;
		locations.color = -1;
		locations.texCoord = -1;
		locations.matAmbient = -1;
		locations.matDiffuse = -1;
		locations.matSpecular = -1;
		locations.matShininess = -1;
		locations.texSampler = -1;
		locations.hasTexture = -1;
		locations.moonLightAmbient = -1;
		locations.moonLightDiffuse = -1;
		locations.moonLightDirection = -1;
		locations.moonLightSpecular = -1;
		locations.fireflyAmbient = -1;
		locations.fireflyDiffuse = -1;
		locations.fireflySpecular = -1;
		locations.fireflyPosition = -1;
		locations.alpha = -1;
		locations.fireflyConstant = -1;
		locations.fireflyLinear = -1;
		locations.fireflyQuadratic = -1;
		locations.lamp1Ambient = -1;
		locations.lamp1Diffuse = -1;
		locations.lamp1Specular = -1;
		locations.lamp1Position = -1;
		locations.lamp1Constant = -1;
		locations.lamp1Linear = -1;
		locations.lamp1Quadratic = -1;
		locations.lamp1SpotCosCutOff = -1;
		locations.lamp1SpotExponent = -1;
		locations.lamp1Direction = -1;
		locations.UVMatrix = -1;
		locations.lanternPosition = -1;
		locations.lanternAmbient = -1;
		locations.lanternDiffuse = -1;
		locations.lanternSpecular = -1;
		locations.lanternConstant = -1;
		locations.lanternLinear = -1;
		locations.lanternQuadratic = -1;
	}

} ShaderProgram;

/**
 * \brief Geometry of an object (vertices, triangles).
 */
typedef struct _ObjectGeometry {
	GLuint        vertexBufferObject;   ///< identifier for the vertex buffer object
	GLuint        elementBufferObject;  ///< identifier for the element buffer object
	GLuint        vertexArrayObject;    ///< identifier for the vertex array object
	GLuint        normalBufferObject;
	GLuint        texCoordBufferObject;
	unsigned int  numTriangles;         ///< number of triangles in the mesh
	GLuint        texture;

} ObjectGeometry;

typedef struct _ObjectMaterial {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
	//hasTexture should probably be here

} ObjectMaterial;

class ObjectInstance;
/**
 * \brief Linear representation of the scene objects.  The objects themselves may represent the subtrees.
 */
typedef std::vector<ObjectInstance*> ObjectList;  

class ObjectInstance {

protected:

	ObjectGeometry* geometry;
	ObjectMaterial* material;
	glm::mat4		localModelMatrix;
	glm::mat4		globalModelMatrix;
	glm::mat4		UVMatrix;

	glm::mat4       modelRotationOffset; //object specific rotation offset (for weird models)

	glm::vec3		positionInWorld;
	glm::vec3		direction;
	glm::vec3		upVector;
	float			scale;
	float alpha;

	// dynamic objects
	bool			isDynamic;
	bool			hasAnimatedTexture;
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
		material = nullptr;
		shaderProgram = shdrPrg;
		positionInWorld = glm::vec3(0.0f, 0.0f, 0.0f);
		isDynamic = false;
		//TODO set speed!!!
		speed = 0.7f; 
		upVector = glm::vec3(0.0f, 1.0f, 0.0f);
		direction = glm::vec3(0.0f, 0.0f, 1.0f);
		startPosition = glm::vec3(0.0f, 0.0f, 0.0f);
		modelRotationOffset = glm::mat4(1.0f);
		alpha = 1.0f;
		scale = 1.0f;
		UVMatrix = glm::mat4(1.0f);
		hasAnimatedTexture = true;
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

		localModelMatrix = matrix * modelRotationOffset;
	}

	virtual void setScale(float sc) {
		scale = sc;
		transformObject();
	}

	virtual void setPosition(glm::vec3 new_position) {
		//TODO jaky je presne rozdil mezi globalModelMatrix a localModelMatrix?
		positionInWorld = new_position;
		transformObject();
	}

	virtual void setIsDynamic(bool parameter) {
		isDynamic = parameter;
		transformObject();
	}

	virtual void setDirection(glm::vec3 dir) {
		direction = glm::normalize(dir);

		glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

		if (abs(direction.y) > 0.999f) {
			// If pointing straight up/down, use the Z-axis as the reference instead
			worldUp = glm::vec3(0.0f, 0.0f, 1.0f);
		}

		//update upvector accordingly
		glm::vec3 right = glm::normalize(glm::cross(worldUp, direction));
		upVector = glm::cross(direction, right);

		transformObject();
	}

	virtual void setUpVector(glm::vec3 upVec) {
		upVector = glm::normalize(upVec);
		transformObject();
	}

	virtual void setStartPosition(glm::vec3 pos) {
		startPosition = pos;
		positionInWorld = pos;
		transformObject();
	}

	virtual void setModelRotationOffset(float degrees, glm::vec3 axis) {
		modelRotationOffset = glm::rotate(glm::mat4(1.0f), glm::radians(degrees), axis);
		transformObject();
	}

	virtual void setAlpha(float a) {
		alpha = a;
		transformObject();
	}

	virtual void setUVMatrix(glm::mat4 matrix) {
		UVMatrix = matrix;
	}

	virtual void setHasAnimatedTexture(bool parameter) {
		hasAnimatedTexture = parameter;
	}
	
	virtual void setColor(glm::vec3 c) {
		material->diffuse = c;
	}
};
