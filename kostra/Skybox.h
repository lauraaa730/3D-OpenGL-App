#include <string>
#include <glm/mat4x4.hpp>
#include "pgr.h"
#include "object.h"
#include <iostream>
#pragma once

extern const std::string skyboxVertexShaderSrc;
extern const std::string skyboxFragmentShaderSrc;

struct SkyboxShaderProgram {
    // OpenGL program ID
    GLuint program = 0;

    // vertex attributes
    GLint positionLocation = -1;

    // uniforms
    GLint viewMatrixLocation = -1;
    GLint projectionMatrixLocation = -1;
    GLint skyboxSamplerLocation = -1;

    GLint viewNoTranslationLocation = -1;

    bool initialized = false;
};

extern SkyboxShaderProgram skyboxShaderProgram;

class Skybox
{
public:
	Skybox();
    void init();
	void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);

private:
    ObjectGeometry* geometry;
	void initSkyboxGeometry(ObjectGeometry** geometry);
    GLuint loadCubemap(const std::vector<std::string>& faces);
};

