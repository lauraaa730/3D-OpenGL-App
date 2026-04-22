#include "Skybox.h"

/*"out vec3 texCoord;\n"

    "  texCoord = position;\n"*/

const std::string skyboxVertexShaderSrc(
	"#version 140\n"
	"\n"
	"in vec3 position;\n"
    "\n"
    "uniform mat4 viewNoTranslation;\n"
    "uniform mat4 projection;\n"
    "\n"
    "void main() {\n"
    "  gl_Position = projection * viewNoTranslation * vec4(position, 1.0);\n"
    "}\n"
);

/*"in vec3 texCoord;\n"
	"uniform samplerCube skyboxSampler;\n"*/
const std::string skyboxFragmentShaderSrc(
	"#version 140\n"
	"\n"
	"out vec4 color_f;\n"
	"\n"
	"void main() {\n"
	"  color_f = vec4(1.0, 1.0, 1.0, 1.0);\n"
	"}\n"
);

SkyboxShaderProgram skyboxShaderProgram;

Skybox::Skybox() {
    geometry = nullptr;
}

void Skybox::init() {
    initSkyboxGeometry(&geometry);
}

void Skybox::draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
    glUseProgram(skyboxShaderProgram.program);

    // 1. Remove translation from view matrix (skybox stays at camera position)
    glm::mat4 viewNoTranslation = glm::mat4(glm::mat3(viewMatrix));

    // 2. Upload uniforms
    glUniformMatrix4fv(
        skyboxShaderProgram.viewNoTranslationLocation,
        1,
        GL_FALSE,
        glm::value_ptr(viewNoTranslation)
    );

    glUniformMatrix4fv(
        skyboxShaderProgram.projectionMatrixLocation,
        1,
        GL_FALSE,
        glm::value_ptr(projectionMatrix)
    );

    // 3. Bind cubemap texture (stored in geometry)
    /*glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &((*geometry)->texture));
    glBindTexture(GL_TEXTURE_CUBE_MAP, geometry->texture);*/

    //glUniform1i(skyboxShaderProgram.skyboxSamplerLocation, 0);

    // 4. Draw cube
    glBindVertexArray(geometry->vertexArrayObject);
    glDrawElements(GL_TRIANGLES, geometry->numTriangles * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // 5. Cleanup
    glUseProgram(0);

}

void Skybox::initSkyboxGeometry(ObjectGeometry** geometry) {
    *geometry = new ObjectGeometry;

    // cube vertices (positions only)
    float vertices[] = {
        -1, -1, -1,
         1, -1, -1,
         1,  1, -1,
        -1,  1, -1,
        -1, -1,  1,
         1, -1,  1,
         1,  1,  1,
        -1,  1,  1
    };

    // indices (12 triangles)
    unsigned int indices[] = {
        0,1,2, 2,3,0, // back
        4,5,6, 6,7,4, // front
        0,4,7, 7,3,0, // left
        1,5,6, 6,2,1, // right
        3,2,6, 6,7,3, // top
        0,1,5, 5,4,0  // bottom
    };

    // VBO
    glGenBuffers(1, &((*geometry)->vertexBufferObject));
    glBindBuffer(GL_ARRAY_BUFFER, (*geometry)->vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // EBO
    glGenBuffers(1, &((*geometry)->elementBufferObject));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*geometry)->elementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // VAO
    glGenVertexArrays(1, &((*geometry)->vertexArrayObject));
    glBindVertexArray((*geometry)->vertexArrayObject);

    glBindBuffer(GL_ARRAY_BUFFER, (*geometry)->vertexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*geometry)->elementBufferObject);

    if (skyboxShaderProgram.positionLocation != -1) {
        glEnableVertexAttribArray(skyboxShaderProgram.positionLocation);
        glVertexAttribPointer(
            skyboxShaderProgram.positionLocation,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            0
        );
    }

    glBindVertexArray(0);

    (*geometry)->numTriangles = 12;
}