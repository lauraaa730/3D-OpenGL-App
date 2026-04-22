#pragma once
#include <glm/glm.hpp>
class Camera
{
public:
	Camera();
	float speed;
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 upVector;
	float yaw; //elevation
	float pitch; //rotation around y axis

	void Move(glm::vec3 dir);
	
};

