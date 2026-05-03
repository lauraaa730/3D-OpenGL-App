#pragma once
#include "pgr.h"

/*
* pure data class
* 
*/

struct Model {
	const char* obj_address;
	const char* texture_address;
	glm::vec3 position;
	glm::vec3 direction;
	float scale;
	bool isDynamic;
};

struct HardCodedModel {
	glm::vec3 position;
	glm::vec3 direction;

	float scale;
	bool isDynamic;
	float alpha;

	//geometry
	const float * vertices;
	const unsigned int * indeces;
	int trianglesNum;
	int verticesNum;

	//material
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
};

const float stone_vertices[] = {
	// -------- top ring --------
 0.00f,  0.90f,  0.00f,   0.70f, 0.70f, 0.70f,
 0.35f,  0.78f,  0.18f,   0.62f, 0.62f, 0.62f,
 0.18f,  0.80f,  0.42f,   0.68f, 0.68f, 0.68f,
-0.20f,  0.76f,  0.38f,   0.58f, 0.58f, 0.58f,
-0.42f,  0.74f,  0.10f,   0.65f, 0.65f, 0.65f,
-0.34f,  0.77f, -0.22f,   0.60f, 0.60f, 0.60f,
-0.08f,  0.81f, -0.40f,   0.72f, 0.72f, 0.72f,
 0.26f,  0.79f, -0.34f,   0.57f, 0.57f, 0.57f,
 0.44f,  0.75f, -0.06f,   0.66f, 0.66f, 0.66f,

 // -------- middle ring --------
  0.62f,  0.35f,  0.10f,   0.52f, 0.52f, 0.52f,
  0.44f,  0.30f,  0.48f,   0.60f, 0.60f, 0.60f,
  0.10f,  0.28f,  0.62f,   0.48f, 0.48f, 0.48f,
 -0.32f,  0.33f,  0.56f,   0.64f, 0.64f, 0.64f,
 -0.58f,  0.31f,  0.20f,   0.55f, 0.55f, 0.55f,
 -0.55f,  0.27f, -0.22f,   0.46f, 0.46f, 0.46f,
 -0.24f,  0.29f, -0.56f,   0.63f, 0.63f, 0.63f,
  0.18f,  0.32f, -0.60f,   0.50f, 0.50f, 0.50f,
  0.56f,  0.30f, -0.30f,   0.58f, 0.58f, 0.58f,

  // -------- lower ring --------
   0.52f, -0.12f,  0.14f,   0.42f, 0.42f, 0.42f,
   0.34f, -0.16f,  0.44f,   0.49f, 0.49f, 0.49f,
   0.02f, -0.20f,  0.52f,   0.38f, 0.38f, 0.38f,
  -0.30f, -0.14f,  0.42f,   0.55f, 0.55f, 0.55f,
  -0.50f, -0.18f,  0.08f,   0.41f, 0.41f, 0.41f,
  -0.42f, -0.22f, -0.26f,   0.47f, 0.47f, 0.47f,
  -0.14f, -0.18f, -0.46f,   0.36f, 0.36f, 0.36f,
   0.24f, -0.16f, -0.48f,   0.51f, 0.51f, 0.51f,
   0.48f, -0.14f, -0.18f,   0.44f, 0.44f, 0.44f,

   // -------- bottom --------
	0.00f, -0.62f,  0.00f,   0.30f, 0.30f, 0.30f,
};

const unsigned int stone_indices[] = {
	// top cap
0,1,2,   0,2,3,   0,3,4,
0,4,5,   0,5,6,   0,6,7,
0,7,8,   0,8,1,

// top ring -> middle ring
1,9,10,   1,10,2,
2,10,11,  2,11,3,
3,11,12,  3,12,4,
4,12,13,  4,13,5,
5,13,14,  5,14,6,
6,14,15,  6,15,7,
7,15,16,  7,16,8,
8,16,17,  8,17,1,
1,17,9,

// middle ring -> lower ring
9,18,19,   9,19,10,
10,19,20,  10,20,11,
11,20,21,  11,21,12,
12,21,22,  12,22,13,
13,22,23,  13,23,14,
14,23,24,  14,24,15,
15,24,25,  15,25,16,
16,25,26,  16,26,17,
17,26,18,  17,18,9,

// bottom taper
18,27,19,
19,27,20,
20,27,21,
21,27,22,
22,27,23,
23,27,24,
24,27,25,
25,27,26,
26,27,18

};

const float pond_vertices[] = {
	3.0f, 0.0f, 0.0f,		0.0f, 0.1f, 1.0f,
	-3.0f, 0.0f, 0.0f,		0.0f, 0.1f, 1.0f,
	0.0f, 0.0f, 3.0f,		0.0f, 0.1f, 1.0f,
	0.0f, 0.0f, -3.0f,		0.0f, 0.1f, 1.0f
};

const unsigned int pond_indices[] = {
	0, 1, 2,
	0, 1, 3
};

const Model myModels[] = { 
	{ "myModels/Fish/fish.obj", "myModels/Fish/fish.jpg", glm::vec3(0.0f,0.0f,0.0f),glm::vec3(3.0f,2.0,3.0), 0.3f, true },
	{ "myModels/Hibiscous/hibiscous.obj", "myModels/Hibiscous/hibiscous_color.jpg", glm::vec3(0.0f,1.0f,0.0f),glm::vec3(0.0,1.0,0.0), 1.0f, false },
	{ "myModels/Stone/Stone.obj", "myModels/Stone/Diffuse.tga", glm::vec3(0.0f,-10.0f,0.0f),glm::vec3(1.0,1.0,2.0), 10.0f, false },
	{ "myModels/firefly/firefly.obj", "myModels/firefly/fly.png", glm::vec3(3.0f,3.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f), 1.0f, true }

};

const HardCodedModel HardCodedObjects[] = {
	{	//stone
		glm::vec3(0.0f),
		glm::vec3(1.0f),
		1.0f,
		false,
		1.0f,
		stone_vertices,
		stone_indices,
		52,
		28,
		glm::vec3(0.2f, 0.2f, 0.2f), //ambient
		glm::vec3(0.5f, 0.5f, 0.5f), //diffuse
		glm::vec3(0.1f, 0.1f, 0.1f), //specular
		8.0f //shininess
	},

	{ //pond
		glm::vec3(0.0f, -0.1f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		1.0f,
		false,
		1.0f,
		pond_vertices,
		pond_indices,
		2,
		4,
		glm::vec3(1.0f, 1.0f, 1.0f), //ambient
		glm::vec3(0.0f, 0.1f, 1.0f), //diffuse
		glm::vec3(1.0f), //specular
		32.0f //shininess
		
	}
};

