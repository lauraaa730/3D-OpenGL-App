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
	{ "myModels/Fish/fish.obj", "myModels/Fish/fish.jpg", glm::vec3(0.0f,0.0f,0.0f),glm::vec3(3.0f,2.0,3.0), 0.3f, true },
	{ "myModels/Hibiscous/hibiscous.obj", "myModels/Hibiscous/hibiscous_color.jpg", glm::vec3(0.0f,5.0f,0.0f),glm::vec3(3.0,2.0,3.0), 1.0f, false },
	{ "myModels/Stone/Stone.obj", "myModels/Stone/Diffuse.tga", glm::vec3(0.0f,-10.0f,0.0f),glm::vec3(1.0,1.0,2.0), 10.0f, false }

};