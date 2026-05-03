#pragma once

#include "object.h"
#include "pgr.h"
#include <string>

class Billboard : public ObjectInstance {
public:
    int currentFrame;
    float timePerFrame;
    int totalFrames;

    //constructor
    Billboard(const std::string& texturePath, ShaderProgram* shdrPrg);

    //destructor
    ~Billboard();

    //custom draw method with additive blending
    virtual void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) override;
    virtual void update(const float elapsedTime, const glm::mat4* parentModelMatrix) override;
};