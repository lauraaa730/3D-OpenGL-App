#include "Skybox.h"



const std::string skyboxVertexShaderSrc(
	"#version 140\n"
	"\n"
	"in vec3 position;\n"
    "out vec3 texCoord;\n"
    "\n"
    "uniform mat4 viewNoTranslation;\n"
    "uniform mat4 projection;\n"
    "\n"
    "void main() {\n"
    "  texCoord = vec3(position.x, -position.y, position.z);\n"
    "  gl_Position = projection * viewNoTranslation * vec4(position, 1.0);\n"
    "}\n"
);

const std::string skyboxFragmentShaderSrc(
	"#version 140\n"
	"\n"
    "in vec3 texCoord;\n"
    "uniform samplerCube skyboxSampler;\n"
	"out vec4 color_f;\n"
	"\n"
	"void main() {\n"
	"  color_f = texture(skyboxSampler,texCoord);\n"
	"}\n"
);

SkyboxShaderProgram skyboxShaderProgram;

Skybox::Skybox() {
    geometry = nullptr;
}

void Skybox::init() {
    initSkyboxGeometry(&geometry);
    std::vector<std::string> faces = {
        "resources/skybox/night-no-moon/jettelly_no_moon_LEFT.png",  // GL_TEXTURE_CUBE_MAP_POSITIVE_X //LEFT
        "resources/skybox/night-no-moon/jettelly_no_moon_RIGHT.png",  // GL_TEXTURE_CUBE_MAP_NEGATIVE_X //RIGHT
        "resources/skybox/night-no-moon/jettelly_no_moon_DOWN.png",  // GL_TEXTURE_CUBE_MAP_POSITIVE_Y //BOTTOM 
        "resources/skybox/night-no-moon/jettelly_moon_UP.png",  // GL_TEXTURE_CUBE_MAP_NEGATIVE_Y //TOP
        "resources/skybox/night-no-moon/jettelly_no_moon_FRONT.png",  // GL_TEXTURE_CUBE_MAP_POSITIVE_Z // FRONT
        "resources/skybox/night-no-moon/jettelly_no_moon_BACK.png"   // GL_TEXTURE_CUBE_MAP_NEGATIVE_Z //BACK
    };
    
    geometry->texture = loadCubemap(faces);
}

void Skybox::draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
    glUseProgram(skyboxShaderProgram.program);

    //remove translation from view matrix (skybox stays at camera position)
    glm::mat4 viewNoTranslation = glm::mat4(glm::mat3(viewMatrix));

    //pload uniforms
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

    //bind cubemap texture (stored in geometry)
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, geometry->texture);
    glUniform1i(skyboxShaderProgram.skyboxSamplerLocation, 0);

    //draw cube
    glBindVertexArray(geometry->vertexArrayObject);
    glDrawElements(GL_TRIANGLES, geometry->numTriangles * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    //cleanup
    glUseProgram(0);
}

void Skybox::initSkyboxGeometry(ObjectGeometry** geometry) {
    *geometry = new ObjectGeometry;

    //cube vertices
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

    //indices (12 triangles) = 6 squares
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

GLuint Skybox::loadCubemap(const std::vector<std::string>& faces) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    for (unsigned int i = 0; i < faces.size(); i++) {
        if (!pgr::loadTexImage2D(faces[i], GL_TEXTURE_CUBE_MAP_POSITIVE_X + i)) {
            std::cerr << "Failed to load cubemap texture at: " << faces[i] << std::endl;
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}