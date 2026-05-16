//----------------------------------------------------------------------------------------
/**
 * \file    Parameters.h
 * \author  Laura Katerina Dudkova
 * \date    2026/05/11
 * \brief   Global constants and parameters for the scene bounds, window settings, input mapping, and lighting.
 */

#pragma once
#include "pgr.h"

constexpr float SCENE_WIDTH = 7.5f;  ///< \brief Boundary width of the scene limits.
constexpr float SCENE_HEIGHT = 10.0f; ///< \brief Boundary height of the scene limits.
constexpr float SCENE_DEPTH = 7.5f;   ///< \brief Boundary depth of the scene limits.

constexpr float FAR_CLIPPING_PLANE = 100.f; ///< \brief Distance to the far clipping plane for camera projection.

constexpr int WINDOW_WIDTH = 1600; ///< \brief Initial width of the application window.
constexpr int WINDOW_HEIGHT = 900; ///< \brief Initial height of the application window.
constexpr char WINDOW_TITLE[] = "PGR: dudkolau"; ///< \brief Title of the application window.

/**
 * \brief Enumeration of indices for the keyboard input mapping array.
 */
enum {
	KEY_LEFT,          ///< \brief Index for moving left.
	KEY_RIGHT,         ///< \brief Index for moving right.
	KEY_FORWARD,       ///< \brief Index for moving forward.
	KEY_BACKWARDS,     ///< \brief Index for moving backward.
	KEY_UP,            ///< \brief Index for moving up.
	KEY_DOWN,          ///< \brief Index for moving down.
	KEY_SWITCH_CAMERA, ///< \brief Index for switching the camera mode.
	KEY_GET_STATUS,    ///< \brief Index for printing the current status.
	CLICK_KEY,         ///< \brief Index for a mouse click action.
	KEYS_COUNT         ///< \brief Total number of tracked key actions.
};

bool keyMap[KEYS_COUNT]; ///< \brief Array storing the current state (pressed/released) of mapped keys.


//LIGHTS =======================================================================

//Moon Light
glm::vec3 moonLightAmbient = glm::vec3(0.5f, 0.5f, 1.0f);             ///< \brief Ambient color component of the moon light.
glm::vec3 moonLightDiffuse = glm::vec3(0.6f, 0.6f, 0.9f);             ///< \brief Diffuse color component of the moon light.
glm::vec3 moonLightSpecular = glm::vec3(1.0f, 1.0f, 1.0f);            ///< \brief Specular color component of the moon light.
glm::vec3 moonLightDirection = glm::normalize(glm::vec3(1.0f, -1.0f, -0.1f)); ///< \brief Normalized direction vector of the moon light.

//Firefly
glm::vec3 fireflyPosition = glm::vec3(0.2f, 1.0f, -0.8f);             ///< \brief 3D position of the firefly point light.
glm::vec3 fireflyAmbient = glm::vec3(0.4f, 0.4f, 0.4f);               ///< \brief Ambient color component of the firefly light.
glm::vec3 fireflyDiffuse = glm::vec3(8.0f, 8.0f, 0.0f);               ///< \brief Diffuse color component of the firefly light.
glm::vec3 fireflySpecular = glm::vec3(0.5f, 0.5f, 0.0f);              ///< \brief Specular color component of the firefly light.
float fireflyConstant = 1.0f;                                         ///< \brief Constant attenuation factor for the firefly light.
float fireflyLinear = 0.9f;                                           ///< \brief Linear attenuation factor for the firefly light.
float fireflyQuadratic = 0.8f;                                        ///< \brief Quadratic attenuation factor for the firefly light.

unsigned int maxGlowCounter = 16;
//fps for firefly glow
float fireflyTimePerFrame = 1.0f / 10.0f;							  ///< \brief Time per frame in dimming light animation.

//Lantern
glm::vec3 lanternPosition = glm::vec3(3.3f, 0.64f, -5.12f);           ///< \brief 3D position of the lantern point light.
glm::vec3 lanternAmbient = glm::vec3(0.4f, 0.4f, 0.4f);               ///< \brief Ambient color component of the lantern light.
glm::vec3 lanternDiffuse = glm::vec3(6.0f, 6.0f, 0.1f);               ///< \brief Diffuse color component of the lantern light.
glm::vec3 lanternSpecular = glm::vec3(0.5f, 0.5f, 0.1f);              ///< \brief Specular color component of the lantern light.
float lanternConstant = 1.0f;                                         ///< \brief Constant attenuation factor for the lantern light.
float lanternLinear = 0.9f;                                           ///< \brief Linear attenuation factor for the lantern light.
float lanternQuadratic = 0.8f;                                        ///< \brief Quadratic attenuation factor for the lantern light.

//Lamp
glm::vec3 lamp_1Ambient = glm::vec3(0.05f, 0.05f, 0.05f);                ///< \brief Ambient color component of the lamp spotlight.
glm::vec3 lamp_1Diffuse = glm::vec3(0.9f, 0.9f, 1.0f);                ///< \brief Diffuse color component of the lamp spotlight.
glm::vec3 lamp_1Specular = glm::vec3(1.0f, 1.0f, 1.0f);               ///< \brief Specular color component of the lamp spotlight.
glm::vec3 lamp_1Position = glm::vec3(0.0f, 5.0f, 0.0f);               ///< \brief 3D position of the lamp spotlight.
glm::vec3 lamp_1Direction = glm::vec3(-0.2f, -1.0f, -1.0f);           ///< \brief Direction vector the lamp spotlight is pointing.
float lamp_1Constant = 1.0f;                                          ///< \brief Constant attenuation factor for the lamp spotlight. The smaller the number, the further it can reach.
float lamp_1Linear = 0.045f;                                          ///< \brief Linear attenuation factor for the lamp spotlight. The smaller the number, the further it can reach.
float lamp_1Quadratic = 0.0075f;                                      ///< \brief Quadratic attenuation factor for the lamp spotlight. The smaller the number, the further it can reach.
float lamp_1SpotCosCutOff = 0.965f;                                   ///< \brief Cosine of the half-angle defining the spotlight cone. The smaller the number, the bigger the cone.
float lamp_1SpotExponent = 70.0;                                      ///< \brief Exponent defining light falloff within the spotlight cone. THe bigger the number, the bigger falloff.

// =============================================================================

// OBJECT SPECIFIC CONSTANTS ===================================================

constexpr float stoneColliderRadius = 0.15f;                          ///< \brief Radius defining the collision boundary for the rigid stone object.