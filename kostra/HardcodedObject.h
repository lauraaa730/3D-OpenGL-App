#pragma once

#include "object.h"
#include "SceneObjectsData.h"

class HardcodedObject : public ObjectInstance
{
public:

	HardcodedObject(ShaderProgram* shdrPrg = nullptr, const HardCodedModel* model = nullptr);
	~HardcodedObject();

	void update(float elapsedTime, const glm::mat4* parentModelMatrix) override;
	void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) override;

private:

	bool initialized;  ///< object has the shader with defined locations
};



