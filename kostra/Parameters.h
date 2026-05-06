#pragma once
#include "pgr.h"

constexpr float SCENE_WIDTH = 1.0f;
constexpr float SCENE_HEIGHT = 1.0f;
constexpr float SCENE_DEPTH = 1.0f;

constexpr float FAR_CLIPPING_PLANE = 100.f;

constexpr int WINDOW_WIDTH = 1080;
constexpr int WINDOW_HEIGHT = 850;
constexpr char WINDOW_TITLE[] = "PGR: dudkolau";

enum { KEY_LEFT, KEY_RIGHT, KEY_FORWARD, KEY_BACKWARDS, KEY_UP, KEY_DOWN, KEY_SWITCH_CAMERA, KEY_GET_STATUS, KEYS_COUNT };

bool keyMap[KEYS_COUNT];


//LIGHTS
glm::vec3 moonLightAmbient = glm::vec3(0.3f, 0.3f, 0.9f);
glm::vec3 moonLightDiffuse = glm::vec3(0.6f, 0.6f, 0.9f);
glm::vec3 moonLightSpecular = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 moonLightDirection = glm::normalize(glm::vec3(1.0f, -1.0f, -0.1f));

//Firefly
glm::vec3 fireflyPosition = glm::vec3(0.2f, 1.0f, -0.8f); //
glm::vec3 fireflyAmbient = glm::vec3(0.4f, 0.4f, 0.4f);
glm::vec3 fireflyDiffuse = glm::vec3(8.0f, 8.0f, 0.0f);
glm::vec3 fireflySpecular = glm::vec3(0.5f, 0.5f, 0.0f);
float fireflyConstant = 1.0f;
float fireflyLinear = 0.9f;
float fireflyQuadratic = 0.8f;

//Lantern
glm::vec3 lanternPosition = glm::vec3(3.3f, 0.64f, -5.12f); //
glm::vec3 lanternAmbient = glm::vec3(0.4f, 0.4f, 0.4f);
glm::vec3 lanternDiffuse = glm::vec3(6.0f, 6.0f, 0.1f);
glm::vec3 lanternSpecular = glm::vec3(0.5f, 0.5f, 0.1f);
float lanternConstant = 1.0f;
float lanternLinear = 0.9f;
float lanternQuadratic = 0.8f;

//Lamp
glm::vec3 lamp_1Ambient = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 lamp_1Diffuse = glm::vec3(0.9f, 0.9f, 1.0f);    
glm::vec3 lamp_1Specular = glm::vec3(1.0f, 1.0f, 1.0f);      
glm::vec3 lamp_1Position = glm::vec3(0.0f, 5.0f, 0.0f);      
glm::vec3 lamp_1Direction = glm::vec3(-0.2f, -1.0f, -1.0f);    
float lamp_1Constant = 1.0f;
float lamp_1Linear = 0.045f;                      
float lamp_1Quadratic = 0.0075f;
float lamp_1SpotCosCutOff = 0.965f;               
float lamp_1SpotExponent = 40.0;                 

