#pragma once
#include "pgr.h"

struct Light {
    glm::vec3 ambient = glm::vec3(1.0f);
    glm::vec3 diffuse = glm::vec3(1.0f);
    glm::vec3 specular = glm::vec3(1.0f);
};

struct DirectionalLight : Light {
    glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f);
};

struct PointLight : Light {
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
};

struct SpotLight : Light {
    glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f);;
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    float spotCosCutOff = 0.0f; //cosine of the spotlight's half angle
    float spotExponent = 0.5f; // distribution of the light energy within the spotlight's cone (center -> cone's edge)
};

struct SceneLights {
    DirectionalLight moonLight;
    PointLight firefly;
    SpotLight lamp_1;
};