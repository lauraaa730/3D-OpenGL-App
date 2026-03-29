#pragma once
#include <glm/glm.hpp>
class Camera
{
public:
	Camera();
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 upVector;

	void Move(glm::vec3 dir);
	
};

