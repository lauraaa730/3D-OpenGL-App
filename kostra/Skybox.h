//----------------------------------------------------------------------------------------
/**
 * \file    Skybox.h
 * \brief   Header file for the Skybox class and related shader program structures.
 * \author  Laura Katerina Dudkova
 * \date    2026/05/11
 */

#pragma once
#include <string>
#include <glm/mat4x4.hpp>
#include "pgr.h"
#include "object.h"
#include <iostream>

extern const std::string skyboxVertexShaderSrc; ///< \brief Source code for the skybox vertex shader.
extern const std::string skyboxFragmentShaderSrc; ///< \brief Source code for the skybox fragment shader.

/**
 * \struct SkyboxShaderProgram
 * \brief Encapsulates the shader program and its uniform/attribute locations for rendering the skybox.
 */
struct SkyboxShaderProgram {
    // OpenGL program ID
    GLuint program = 0; ///< \brief OpenGL shader program ID.

    // vertex attributes
    GLint positionLocation = -1; ///< \brief Location of the position vertex attribute.

    // uniforms
    GLint viewMatrixLocation = -1; ///< \brief Location of the view matrix uniform (unused, viewNoTranslation used instead).
    GLint projectionMatrixLocation = -1; ///< \brief Location of the projection matrix uniform.
    GLint skyboxSamplerLocation = -1; ///< \brief Location of the cubemap texture sampler uniform.

    GLint viewNoTranslationLocation = -1; ///< \brief Location of the modified view matrix uniform (translation removed).

    bool initialized = false; ///< \brief Flag indicating whether the shader program is successfully initialized.
};

extern SkyboxShaderProgram skyboxShaderProgram; ///< \brief Global instance of the skybox shader program.

/**
 * \class Skybox
 * \brief Handles the creation, texturing, and rendering of a 3D skybox environment.
 */
class Skybox
{
public:
    /**
     * \brief Default constructor. Initializes the geometry pointer to nullptr.
     */
    Skybox();

    /**
     * \brief Initializes the skybox geometry and loads the cubemap textures.
     */
    void init();

    /**
     * \brief Draws the skybox using the provided view and projection matrices.
     * \param viewMatrix The current camera view matrix.
     * \param projectionMatrix The current camera projection matrix.
     */
    void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);

private:
    ObjectGeometry* geometry; ///< \brief Pointer to the object geometry structure containing VBO, VAO, and EBO.

    /**
     * \brief Initializes the vertex data, VBO, EBO, and VAO for the skybox cube.
     * \param geometry Pointer to the ObjectGeometry pointer to be allocated and initialized.
     */
    void initSkyboxGeometry(ObjectGeometry** geometry);

    /**
     * \brief Loads six 2D textures to create an OpenGL cubemap.
     * \param faces A vector of 6 strings representing the file paths to the textures (in correct GL_TEXTURE_CUBE_MAP order).
     * \return The generated OpenGL texture ID for the cubemap.
     */
    GLuint loadCubemap(const std::vector<std::string>& faces);
};