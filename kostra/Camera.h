#pragma once
#include <glm/glm.hpp>
#include <iostream>

enum Mode
{
	FREE_LOOK,
	POSITION_1,
	POSITION_2,
	MODES_NUM,

};

class Camera
{
public:
	Camera();

	float speed;
	int currMode;

	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 upVector;

	glm::vec3 position1;
	glm::vec3 position2;

	float yaw; //elevation
	float pitch; //rotation around y axis

	void Move(glm::vec3 dir);
	void switchMode();
	
};

