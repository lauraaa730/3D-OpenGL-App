#pragma once
#include "pgr.h"

/*
* pure data class
* 
*/

struct Model {
	const char* name;
	const char* obj_address;
	const char* texture_address;
	glm::vec3 position;
	glm::vec3 direction;
	float scale;
	bool isDynamic;
	bool hasAnimatedTexture;
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

struct FlatShadedModel {
	glm::vec3 position;
	glm::vec3 direction;

	float scale;
	bool isDynamic;
	float alpha;

	//geometry
	const float* vertices;
	int trianglesNum;

	//material
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
};

struct BillboardModel {
	glm::vec3 position;
	bool isOverlay; //if true - its just a picture on top of the screen and we dont apply pvm

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
	// Row 0 (Z = -3.0)
	 3.0f, 0.02f, -3.0f,    1.0f, 1.0f, 1.0f,
	 2.0f, 0.05f, -3.0f,    1.0f, 1.0f, 1.0f,
	 1.0f, 0.08f, -3.0f,    1.0f, 1.0f, 1.0f,
	 0.0f, 0.04f, -3.0f,    1.0f, 1.0f, 1.0f,
	-1.0f, 0.01f, -3.0f,    1.0f, 1.0f, 1.0f,
	-2.0f, 0.06f, -3.0f,    1.0f, 1.0f, 1.0f,
	-3.0f, 0.03f, -3.0f,    1.0f, 1.0f, 1.0f,

	// Row 1 (Z = -2.0)
	 3.0f, 0.05f, -2.0f,    1.0f, 1.0f, 1.0f,
	 2.0f, 0.09f, -2.0f,    1.0f, 1.0f, 1.0f,
	 1.0f, 0.03f, -2.0f,    1.0f, 1.0f, 1.0f,
	 0.0f, 0.07f, -2.0f,    1.0f, 1.0f, 1.0f,
	-1.0f, 0.02f, -2.0f,    1.0f, 1.0f, 1.0f,
	-2.0f, 0.08f, -2.0f,    1.0f, 1.0f, 1.0f,
	-3.0f, 0.00f, -2.0f,    1.0f, 1.0f, 1.0f,

	// Row 2 (Z = -1.0)
	 3.0f, 0.07f, -1.0f,    1.0f, 1.0f, 1.0f,
	 2.0f, 0.01f, -1.0f,    1.0f, 1.0f, 1.0f,
	 1.0f, 0.06f, -1.0f,    1.0f, 1.0f, 1.0f,
	 0.0f, 0.09f, -1.0f,    1.0f, 1.0f, 1.0f,
	-1.0f, 0.05f, -1.0f,    1.0f, 1.0f, 1.0f,
	-2.0f, 0.02f, -1.0f,    1.0f, 1.0f, 1.0f,
	-3.0f, 0.04f, -1.0f,    1.0f, 1.0f, 1.0f,

	// Row 3 (Z = 0.0)
	 3.0f, 0.01f,  0.0f,    1.0f, 1.0f, 1.0f,
	 2.0f, 0.06f,  0.0f,    1.0f, 1.0f, 1.0f,
	 1.0f, 0.02f,  0.0f,    1.0f, 1.0f, 1.0f,
	 0.0f, 0.08f,  0.0f,    1.0f, 1.0f, 1.0f,
	-1.0f, 0.03f,  0.0f,    1.0f, 1.0f, 1.0f,
	-2.0f, 0.09f,  0.0f,    1.0f, 1.0f, 1.0f,
	-3.0f, 0.05f,  0.0f,    1.0f, 1.0f, 1.0f,

	// Row 4 (Z = 1.0)
	 3.0f, 0.04f,  1.0f,    1.0f, 1.0f, 1.0f,
	 2.0f, 0.02f,  1.0f,    1.0f, 1.0f, 1.0f,
	 1.0f, 0.09f,  1.0f,    1.0f, 1.0f, 1.0f,
	 0.0f, 0.05f,  1.0f,    1.0f, 1.0f, 1.0f,
	-1.0f, 0.01f,  1.0f,    1.0f, 1.0f, 1.0f,
	-2.0f, 0.07f,  1.0f,    1.0f, 1.0f, 1.0f,
	-3.0f, 0.03f,  1.0f,    1.0f, 1.0f, 1.0f,

	// Row 5 (Z = 2.0)
	 3.0f, 0.08f,  2.0f,    1.0f, 1.0f, 1.0f,
	 2.0f, 0.05f,  2.0f,    1.0f, 1.0f, 1.0f,
	 1.0f, 0.03f,  2.0f,    1.0f, 1.0f, 1.0f,
	 0.0f, 0.01f,  2.0f,    1.0f, 1.0f, 1.0f,
	-1.0f, 0.06f,  2.0f,    1.0f, 1.0f, 1.0f,
	-2.0f, 0.04f,  2.0f,    1.0f, 1.0f, 1.0f,
	-3.0f, 0.09f,  2.0f,    1.0f, 1.0f, 1.0f,

	// Row 6 (Z = 3.0)
	 3.0f, 0.03f,  3.0f,    1.0f, 1.0f, 1.0f,
	 2.0f, 0.08f,  3.0f,    1.0f, 1.0f, 1.0f,
	 1.0f, 0.05f,  3.0f,    1.0f, 1.0f, 1.0f,
	 0.0f, 0.02f,  3.0f,    1.0f, 1.0f, 1.0f,
	-1.0f, 0.09f,  3.0f,    1.0f, 1.0f, 1.0f,
	-2.0f, 0.01f,  3.0f,    1.0f, 1.0f, 1.0f,
	-3.0f, 0.06f,  3.0f,    1.0f, 1.0f, 1.0f
};

const unsigned int pond_indices[] = {
	// Row 0
	0, 7, 1, 1, 7, 8,       1, 8, 2, 2, 8, 9,       2, 9, 3, 3, 9, 10,
	3, 10, 4, 4, 10, 11,    4, 11, 5, 5, 11, 12,    5, 12, 6, 6, 12, 13,

	// Row 1
	7, 14, 8, 8, 14, 15,    8, 15, 9, 9, 15, 16,    9, 16, 10, 10, 16, 17,
	10, 17, 11, 11, 17, 18, 11, 18, 12, 12, 18, 19, 12, 19, 13, 13, 19, 20,

	// Row 2
	14, 21, 15, 15, 21, 22, 15, 22, 16, 16, 22, 23, 16, 23, 17, 17, 23, 24,
	17, 24, 18, 18, 24, 25, 18, 25, 19, 19, 25, 26, 19, 26, 20, 20, 26, 27,

	// Row 3
	21, 28, 22, 22, 28, 29, 22, 29, 23, 23, 29, 30, 23, 30, 24, 24, 30, 31,
	24, 31, 25, 25, 31, 32, 25, 32, 26, 26, 32, 33, 26, 33, 27, 27, 33, 34,

	// Row 4
	28, 35, 29, 29, 35, 36, 29, 36, 30, 30, 36, 37, 30, 37, 31, 31, 37, 38,
	31, 38, 32, 32, 38, 39, 32, 39, 33, 33, 39, 40, 33, 40, 34, 34, 40, 41,

	// Row 5
	35, 42, 36, 36, 42, 43, 36, 43, 37, 37, 43, 44, 37, 44, 38, 38, 44, 45,
	38, 45, 39, 39, 45, 46, 39, 46, 40, 40, 46, 47, 40, 47, 41, 41, 47, 48
};


const float crystal_vertices[] = {
	//top half
	// Triangle 1
	0.0f, 1.5f, 0.0f,
	1.0f, 0.0f, 0.0f,
	0.5f, 0.0f, 0.866f,

	// Triangle 2
	0.0f, 1.5f, 0.0f,
	0.5f, 0.0f, 0.866f,
	-0.5f, 0.0f, 0.866f,

	// Triangle 3
	0.0f, 1.5f, 0.0f,
	-0.5f, 0.0f, 0.866f,
	-1.0f, 0.0f, 0.0f,

	// Triangle 4
	0.0f, 1.5f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-0.5f, 0.0f, -0.866f,

	// Triangle 5
	0.0f, 1.5f, 0.0f,
	-0.5f, 0.0f, -0.866f,
	0.5f, 0.0f, -0.866f,

	// Triangle 6
	0.0f, 1.5f, 0.0f,
	0.5f, 0.0f, -0.866f,
	1.0f, 0.0f, 0.0f,

	//bottom half
	// Triangle 7
	0.0f, -1.5f, 0.0f,
	0.5f, 0.0f, 0.866f,
	1.0f, 0.0f, 0.0f,

	// Triangle 8
	0.0f, -1.5f, 0.0f,
	-0.5f, 0.0f, 0.866f,
	0.5f, 0.0f, 0.866f,

	// Triangle 9
	0.0f, -1.5f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	-0.5f, 0.0f, 0.866f,

	// Triangle 10
	0.0f, -1.5f, 0.0f,
	-0.5f, 0.0f, -0.866f,
	-1.0f, 0.0f, 0.0f,

	// Triangle 11
	0.0f, -1.5f, 0.0f,
	0.5f, 0.0f, -0.866f,
	-0.5f, 0.0f, -0.866f,

	// Triangle 12
	0.0f, -1.5f, 0.0f,
	1.0f, 0.0f, 0.0f,
	0.5f, 0.0f, -0.866f
};



//dal od vodopadu dopredu - osa z zaporne, doleva - kladne na ose x
const Model myModels[] = { 
	{ "fish", "myModels/Fish/fish.obj", "myModels/Fish/fish.jpg", glm::vec3(0.0f, -0.3f, 0.5f),glm::vec3(3.0f,2.0,3.0), 0.2f, true, false },
	{ "hibiscous", "myModels/Hibiscous/hibiscous.obj", "myModels/Hibiscous/hibiscous_color.jpg", glm::vec3(-2.0f,0.5f,0.0f),glm::vec3(1.0,0.0,0.0), 0.5f, false, false },
	{ "hibiscous", "myModels/Hibiscous/hibiscous.obj", "myModels/Hibiscous/hibiscous_color.jpg", glm::vec3(1.75f,0.7f,1.25f),glm::vec3(0.5,0.0,1.0), 0.5f, false, false },
	{ "crocus", "myModels/Crocus/Crocus.obj", "myModels/Crocus/Crocus.tga", glm::vec3(2.15f,0.5f,0.5f),glm::vec3(0.0,1.0,0.0), 0.2f, false, false },
	{ "kapradi", "myModels/Kapradi/kapradi.obj", "myModels/Kapradi/plane.tga", glm::vec3(1.0f,-1.0f,0.1f),glm::vec3(0.5,0.0,1.0), 1.0f, false, false },
	{ "kapradi", "myModels/Kapradi/kapradi.obj", "myModels/Kapradi/plane.tga", glm::vec3(0.0f,-1.1f,0.75f),glm::vec3(1.0,-0.1,-1.0), 0.7f, false, false },
	{ "kapradi", "myModels/Kapradi/kapradi.obj", "myModels/Kapradi/plane.tga", glm::vec3(-0.5f,-1.1f,-0.1f),glm::vec3(-1.0,0.0,-0.5), 0.9f, false, false },
	//{ "myModels/Stone/Stone.obj", "myModels/Stone/Diffuse.tga", glm::vec3(0.0f,-8.5f,0.0f),glm::vec3(1.0,1.0,2.0), 10.0f, false, false },
	{ "firefly", "myModels/firefly/firefly.obj", "myModels/firefly/fly.png", glm::vec3(0.2f, 1.0f, -0.8f) ,glm::vec3(0.0f,1.0f,0.0f), 0.05f, false, false},
	{ "mine", "myModels/Mine/mine2.obj", "myModels/Mine/mine1.png", glm::vec3(5.0f,1.4f,-5.2f),glm::vec3(0.0f,1.0f,0.0f), 2.5f, false, false},
	{ "cave","myModels/StoneCircle/stone_circle.obj", "myModels/StoneCircle/stone_circle.png", glm::vec3(-0.1f,4.5f,-1.5f),glm::vec3(1.0f,0.0f,0.0f), 20.0f, false, false},
	{ "vodopad", "myModels/Vodopad/vodopad.obj", "myModels/Vodopad/vodopad.png", glm::vec3(0.3f,0.8f,2.2f),glm::vec3(0.2f,-0.5f,1.0f), 1.0f, false, true}
	

};

const HardCodedModel HardCodedObjects[] = {
	{	//stone
		glm::vec3(3.5f,0.5f,-2.0f),
		glm::vec3(1.0f),
		1.0f,
		false,
		1.0f,
		stone_vertices,
		stone_indices,
		52,
		28,
		glm::vec3(0.2f, 0.2f, 0.2f),
		glm::vec3(0.5f, 0.5f, 0.5f), 
		glm::vec3(0.1f, 0.1f, 0.1f), 
		8.0f 
	},


	{ //pond
		glm::vec3(0.1f, 0.0f, -0.5f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		1.0f,
		false,
		0.6f,
		pond_vertices,
		pond_indices,
		72,
		49,
		glm::vec3(0.1f, 0.5f, 0.5f),
		glm::vec3(0.0f, 0.3f, 0.5f),
		glm::vec3(1.0f),
		128.0f 
		
	}
};

const FlatShadedModel FlatShadedObjects[] = {
	{	//crystal
		glm::vec3(-1.5f,1.0f,-5.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		0.2f,
		false,
		0.9f,
		crystal_vertices,
		12,
		glm::vec3(0.2f, 0.2f, 0.2f),
		glm::vec3(1.0f, 0.1f, 0.9f),
		glm::vec3(0.1f, 0.1f, 0.1f),
		8.0f
	},
	{	//crystal
		glm::vec3(-0.5f,1.0f,-5.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		0.2f,
		false,
		0.9f,
		crystal_vertices,
		12,
		glm::vec3(0.2f, 0.2f, 0.2f),
		glm::vec3(1.0f, 0.1f, 0.9f),
		glm::vec3(0.1f, 0.1f, 0.1f),
		8.0f
	},
	{	//crystal
		glm::vec3(-1.0f,1.0f,-4.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		0.2f,
		false,
		0.9f,
		crystal_vertices,
		12,
		glm::vec3(0.2f, 0.2f, 0.2f),
		glm::vec3(1.0f, 0.1f, 0.9f),
		glm::vec3(0.1f, 0.1f, 0.1f),
		8.0f
	}
};


