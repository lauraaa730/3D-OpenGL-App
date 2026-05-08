#pragma once

#include "object.h"
#include "SceneObjectsData.h"

/* 
 * Object with hard-coded vertices and colors 
 * using VBO and VAO for flat shading 

*/

class FlatShadedObject : public ObjectInstance
{
public:

	FlatShadedObject(ShaderProgram* shdrPrg = nullptr, const FlatShadedModel* model = nullptr);
	~FlatShadedObject();

	//void update(float elapsedTime, const glm::mat4* parentModelMatrix) override;
	void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) override;

private:

	bool initialized;  ///< object has the shader with defined locations
};



