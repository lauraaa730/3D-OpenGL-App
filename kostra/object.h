//----------------------------------------------------------------------------------------
/**
 * \file    object.h
 * \author  Laura Katerina Dudkova
 * \date    2026/05/11
 * \brief   Base definitions for shader programs, geometry, materials, and the ObjectInstance base class.
 */

#pragma once
#include "pgr.h"
#include "Spline.h"
#include <iostream>

/**
 * \brief Shader program related stuff (id, locations, ...).
 */
typedef struct _ShaderProgram {
	GLuint program;     ///< \brief Identifier for the shader program.
	bool initialized;   ///< \brief Flag indicating if the shader program was successfully compiled and linked.

	/**
	  * \brief Indices of the vertex shader inputs (locations)
	  */
	struct {
		// vertex attributes locations
		GLint position;  ///< \brief Location of vertex position attribute.
		GLint normal;    ///< \brief Location of vertex normal attribute.
		GLint color;     ///< \brief Location of vertex color attribute.
		GLint alpha;     ///< \brief Location of global alpha uniform.
		GLint texCoord;  ///< \brief Location of texture coordinate attribute.

		// uniforms locations
		GLint PVMmatrix;    ///< \brief Location of the combined PVM matrix uniform.
		GLint texSampler;   ///< \brief Location of the texture sampler uniform.
		GLint hasTexture;   ///< \brief Location of the boolean indicating if texture is used.
		GLint Mmatrix;      ///< \brief Location of the model matrix uniform.
		GLint normalMatrix; ///< \brief Location of the normal transformation matrix uniform.
		GLint Vmatrix;      ///< \brief Location of the view matrix uniform.
		GLint UVMatrix;     ///< \brief Location of the UV coordinate transformation matrix uniform.

		// materials
		GLint matAmbient;   ///< \brief Location of material ambient reflection.
		GLint matDiffuse;   ///< \brief Location of material diffuse reflection.
		GLint matSpecular;  ///< \brief Location of material specular reflection.
		GLint matShininess; ///< \brief Location of material shininess/exponent.

		// moonlight
		GLint moonLightAmbient;   ///< \brief Location of moon light ambient property.
		GLint moonLightDiffuse;   ///< \brief Location of moon light diffuse property.
		GLint moonLightSpecular;  ///< \brief Location of moon light specular property.
		GLint moonLightDirection; ///< \brief Location of moon light direction property.

		// firefly light
		GLint fireflyAmbient;     ///< \brief Location of firefly ambient property.
		GLint fireflyDiffuse;     ///< \brief Location of firefly diffuse property.
		GLint fireflySpecular;    ///< \brief Location of firefly specular property.
		GLint fireflyPosition;    ///< \brief Location of firefly position property.
		GLint fireflyConstant;    ///< \brief Location of firefly constant attenuation.
		GLint fireflyLinear;      ///< \brief Location of firefly linear attenuation.
		GLint fireflyQuadratic;   ///< \brief Location of firefly quadratic attenuation.

		// lamp light
		GLint lamp1Ambient;       ///< \brief Location of lamp ambient property.
		GLint lamp1Diffuse;       ///< \brief Location of lamp diffuse property.
		GLint lamp1Specular;      ///< \brief Location of lamp specular property.
		GLint lamp1Position;      ///< \brief Location of lamp position property.
		GLint lamp1Direction;     ///< \brief Location of lamp direction property.
		GLint lamp1Constant;      ///< \brief Location of lamp constant attenuation.
		GLint lamp1Linear;        ///< \brief Location of lamp linear attenuation.
		GLint lamp1Quadratic;     ///< \brief Location of lamp quadratic attenuation.
		GLint lamp1SpotCosCutOff; ///< \brief Location of lamp spotlight cone cosine cutoff.
		GLint lamp1SpotExponent;  ///< \brief Location of lamp spotlight energy distribution exponent.

		// lantern light
		GLint lanternPosition;    ///< \brief Location of lantern position property.
		GLint lanternAmbient;     ///< \brief Location of lantern ambient property.
		GLint lanternDiffuse;     ///< \brief Location of lantern diffuse property.
		GLint lanternSpecular;    ///< \brief Location of lantern specular property.
		GLint lanternConstant;    ///< \brief Location of lantern constant attenuation.
		GLint lanternLinear;      ///< \brief Location of lantern linear attenuation.
		GLint lanternQuadratic;   ///< \brief Location of lantern quadratic attenuation.

	} locations;

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
	GLuint        vertexBufferObject;   ///< \brief Identifier for the vertex buffer object.
	GLuint        elementBufferObject;  ///< \brief Identifier for the element buffer object.
	GLuint        vertexArrayObject;    ///< \brief Identifier for the vertex array object.
	GLuint        normalBufferObject;   ///< \brief Identifier for the normal buffer object.
	GLuint        texCoordBufferObject; ///< \brief Identifier for the texture coordinate buffer object.
	unsigned int  numTriangles;         ///< \brief Number of triangles in the mesh.
	GLuint        texture;              ///< \brief Identifier for the loaded texture map.

} ObjectGeometry;

/**
 * \brief Material properties of an object used for lighting calculations.
 */
typedef struct _ObjectMaterial {
	glm::vec3 ambient;   ///< \brief Ambient color reflection.
	glm::vec3 diffuse;   ///< \brief Diffuse color reflection.
	glm::vec3 specular;  ///< \brief Specular color reflection.
	float shininess;     ///< \brief Shininess exponent for specular highlights.
	//hasTexture should probably be here

} ObjectMaterial;

class ObjectInstance;
/**
 * \brief Linear representation of the scene objects.  The objects themselves may represent the subtrees.
 */
typedef std::vector<ObjectInstance*> ObjectList;  

/**
 * \class ObjectInstance
 * \brief Base class for all renderable entities in the scene, handling transformations and scenegraph hierarchy.
 */
class ObjectInstance {

protected:

	ObjectGeometry* geometry;           ///< \brief Pointer to object geometry buffers.
	ObjectMaterial* material;           ///< \brief Pointer to object material properties.
	glm::mat4		localModelMatrix;   ///< \brief Model matrix relative to the object's parent.
	glm::mat4		globalModelMatrix;  ///< \brief Final model matrix relative to the world.
	glm::mat4		UVMatrix;           ///< \brief Matrix for texture coordinate transformations.

	glm::mat4       modelRotationOffset; ///< \brief Object specific rotation offset (for weird models).

	glm::vec3		positionInWorld;    ///< \brief Global 3D position.
	glm::vec3		direction;          ///< \brief Forward direction vector.
	glm::vec3		upVector;           ///< \brief Upwards direction vector.
	float			scale;              ///< \brief Uniform scaling factor.
	float alpha;                        ///< \brief Transparency parameter.

	// dynamic objects
	bool			isDynamic;          ///< \brief Flag determining if the object moves via physics/splines.
	bool			hasAnimatedTexture; ///< \brief Flag determining if the object has a scrolling/animated texture.
	float			speed;              ///< \brief Movement speed for dynamic objects.
	glm::vec3		startPosition;      ///< \brief Initial position, used as an offset for spline animation.

	ShaderProgram* shaderProgram;       ///< \brief Pointer to the shader program used to render this object.

	ObjectList children;                ///< \brief List of child objects attached to this instance.

public:

	/**
	 * \brief ObjectInstance constructor. Takes a pointer to the shader and sets default transformation properties.
	 * \param shdrPrg Pointer to the shader program for rendering objects data.
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
	*        Derived classes should also call this method (using ObjectInstance::update()).
	* \param elapsedTime time value in seconds, such as 0.001*glutGet(GLUT_ELAPSED_TIME) (conversion milliseconds => seconds)
	* \param parentModelMatrix parent transformation in the scene-graph subtree
	*/
	virtual void update(const float elapsedTime, const glm::mat4* parentModelMatrix) {
		//update animation in dynamic objects
		if (isDynamic) {
			float splineTime = elapsedTime * speed;

			glm::vec3 pos = findPointOnClosedCurve(splineTime);
			glm::vec3 dir = find_1stDerivative_OnClosedCurve(splineTime);

			setPosition(startPosition + pos);
			setDirection(dir);

			transformObject();
		}

		//if we have parent, multiply parent's matrix with ours
		if (parentModelMatrix != nullptr)
			globalModelMatrix = *parentModelMatrix * localModelMatrix;
		else
			globalModelMatrix = localModelMatrix;

		//update all children
		for (ObjectInstance* child : children) {
			if (child != nullptr)
				child->update(elapsedTime, &globalModelMatrix);
		}
	}

	/**
	 * \brief Draw instance geometry and calls the draw() on child nodes.
	 * \param viewMatrix The active camera view matrix.
	 * \param projectionMatrix The active camera projection matrix.
	 */
	virtual void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {
		//process all children
		for (ObjectInstance* child : children) {
			if (child != nullptr)
				child->draw(viewMatrix, projectionMatrix);
		}
	}

	/**
	 * \brief Calculates the local model matrix based on current position, direction, up vector, and scale.
	 */
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

	/**
	 * \brief Sets the uniform scale of the object.
	 * \param sc The new scaling factor.
	 */
	virtual void setScale(float sc) {
		scale = sc;
		transformObject();
	}

	/**
	 * \brief Sets the world position of the object.
	 * \param new_position The new 3D coordinate.
	 */
	virtual void setPosition(glm::vec3 new_position) {
		//todo jaky je presne rozdil mezi globalModelMatrix a localModelMatrix?
		positionInWorld = new_position;
		transformObject();
	}

	/**
	 * \brief Sets whether the object moves dynamically.
	 * \param parameter True if dynamic, false if static.
	 */
	virtual void setIsDynamic(bool parameter) {
		isDynamic = parameter;
		transformObject();
	}

	/**
	 * \brief Sets the forward direction of the object and recalculates the up vector accordingly.
	 * \param dir The new forward pointing vector.
	 */
	virtual void setDirection(glm::vec3 dir) {
		direction = glm::normalize(dir);

		glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

		if (abs(direction.y) > 0.999f) {
			//if pointing straight up/down, use the Z-axis as the reference instead
			worldUp = glm::vec3(0.0f, 0.0f, 1.0f);
		}

		//update upvector accordingly
		glm::vec3 right = glm::normalize(glm::cross(worldUp, direction));
		upVector = glm::cross(direction, right);

		transformObject();
	}

	/**
	 * \brief Sets the up vector of the object manually.
	 * \param upVec The new up direction vector.
	 */
	virtual void setUpVector(glm::vec3 upVec) {
		upVector = glm::normalize(upVec);
		transformObject();
	}

	/**
	 * \brief Sets the initial starting position used as a base offset for animations.
	 * \param pos The starting 3D coordinate.
	 */
	virtual void setStartPosition(glm::vec3 pos) {
		startPosition = pos;
		positionInWorld = pos;
		transformObject();
	}

	/**
	 * \brief Applies a constant rotation offset to the model matrix, useful for fixing imported model orientations.
	 * \param degrees The angle to rotate.
	 * \param axis The axis to rotate around.
	 */
	virtual void setModelRotationOffset(float degrees, glm::vec3 axis) {
		modelRotationOffset = glm::rotate(glm::mat4(1.0f), glm::radians(degrees), axis);
		transformObject();
	}

	/**
	 * \brief Sets the transparency value for the object.
	 * \param a The alpha value (0.0 to 1.0).
	 */
	virtual void setAlpha(float a) {
		alpha = a;
		transformObject();
	}

	/**
	 * \brief Sets the matrix used to transform texture coordinates.
	 * \param matrix The transformation matrix for UVs.
	 */
	virtual void setUVMatrix(glm::mat4 matrix) {
		UVMatrix = matrix;
	}

	/**
	 * \brief Sets whether the texture should be dynamically animated.
	 * \param parameter True if texture coordinates animate, false if static.
	 */
	virtual void setHasAnimatedTexture(bool parameter) {
		hasAnimatedTexture = parameter;
	}

	/**
	 * \brief Sets the diffuse base color of the object's material.
	 * \param c RGB vector for the color.
	 */
	virtual void setColor(glm::vec3 c) {
		material->diffuse = c;
	}
};
