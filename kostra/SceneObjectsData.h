#pragma once
#include "pgr.h"

struct Model {
	const char* obj_address;
	const char* texture_address;
	glm::vec3 position;
	glm::vec3 direction;
	float scale;
	bool isDynamic;
};

const Model myModels[] = { 
	{ "myModels/Fish/fish.obj", "myModels/Fish/fish.jpg", glm::vec3(0.0f,2.0f,-1.0f),glm::vec3(3.0f,2.0,3.0), 0.3f, true },
	{ "myModels/Hibiscous/hibiscous.obj", "myModels/Hibiscous/hibiscous_color.jpg", glm::vec3(5.0f,3.0f,-1.0f),glm::vec3(3.0,2.0,3.0), 1.0f, false }

};