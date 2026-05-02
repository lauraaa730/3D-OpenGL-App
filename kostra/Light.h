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

struct SceneLights {
    DirectionalLight moonLight;
};