//----------------------------------------------------------------------------------------
/**
 * \file    Light.h
 * \author  Laura Katerina Dudkova
 * \date    2026/05/11
 * \brief   Defines structures for various types of lights used in the scene.
 */

#pragma once
#include "pgr.h"

 /**
  * \brief Base structure for a light source.
  * \details Contains standard ambient, diffuse, and specular color components.
  */
struct Light {
    glm::vec3 ambient = glm::vec3(1.0f);  ///< \brief Ambient light color/intensity.
    glm::vec3 diffuse = glm::vec3(1.0f);  ///< \brief Diffuse light color/intensity.
    glm::vec3 specular = glm::vec3(1.0f); ///< \brief Specular light color/intensity.
};

/**
 * \brief Structure representing a directional light (e.g., the sun or moon).
 * \details Inherits basic light properties and adds a direction vector.
 */
struct DirectionalLight : Light {
    glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f); ///< \brief Direction the light is pointing.
};

/**
 * \brief Structure representing a point light source (e.g., a firefly or lantern).
 * \details Inherits basic light properties and adds position and attenuation parameters.
 */
struct PointLight : Light {
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f); ///< \brief 3D position of the point light.
    float constant = 1.0;                             ///< \brief Constant attenuation factor.
    float linear = 0.09;                              ///< \brief Linear attenuation factor.
    float quadratic = 0.032;                          ///< \brief Quadratic attenuation factor.
};

/**
 * \brief Structure representing a spotlight source (e.g., a street lamp or flashlight).
 * \details Inherits basic light properties and adds position, direction, attenuation, and cone parameters.
 */
struct SpotLight : Light {
    glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f); ///< \brief Direction the spotlight is pointing.
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);   ///< \brief 3D position of the spotlight.
    float spotCosCutOff = 0.0f;                         ///< \brief Cosine of the spotlight's half angle (defines the cone size).
    float spotExponent = 0.5f;                          ///< \brief Distribution of light energy within the cone (center -> edge).
    float constant = 1.0f;                              ///< \brief Constant attenuation factor.
    float linear = 0.09f;                               ///< \brief Linear attenuation factor.
    float quadratic = 0.032f;                           ///< \brief Quadratic attenuation factor.
};

/**
 * \brief Structure containing all specific light sources present in the scene.
 */
struct SceneLights {
    DirectionalLight moonLight; ///< \brief Directional light acting as the moon.
    PointLight firefly;         ///< \brief Point light acting as a glowing firefly.
    PointLight lantern;         ///< \brief Point light acting as a static lantern.
    SpotLight lamp_1;           ///< \brief Spotlight acting as a directional lamp.
};