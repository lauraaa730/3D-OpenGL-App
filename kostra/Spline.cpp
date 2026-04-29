#include "Spline.h"

//This specific curve was inspired by the curve from spline.cpp from the project asteroids 
glm::vec3 controlPoints[] = {
    glm::vec3(0.0f, 0.0f,  0.0f),

    glm::vec3(-3.3f, 0.0f,  3.5f),
    glm::vec3(-6.6f, 0.0f,  3.5f),
    glm::vec3(-10.0f, 0.0f, 0.0f),
    glm::vec3(-6.6f, 0.0f, -3.5f),
    glm::vec3(-3.3f, 0.0f, -3.5f),

    glm::vec3(0.0f, 0.0f,  0.0f),

    glm::vec3(3.3f, 0.0f,  3.5f),
    glm::vec3(6.6f, 0.0f,  3.5f),
    glm::vec3(10.0f, 0.0f, 0.0f),
    glm::vec3(6.6f, 0.0f, -3.5f),
    glm::vec3(3.3f, 0.0f, -3.5f)
};

int segmentsNum = 12;

glm::vec3 findPointOnCurveSegment(
    const glm::vec3& P0,
    const glm::vec3& P1,
    const glm::vec3& P2,
    const glm::vec3& P3,
    const float t) {

    glm::vec3 result(0.0, 0.0, 0.0);

    const float t2 = t * t;
    const float t3 = t2 * t;

    result = 0.5f * (
        P0 * ((-1.0f) * t3 + (2.0f) * t2 + (-1.0f) * t + (0.0f)) +
        P1 * ((3.0f) * t3 + (-5.0f) * t2 + (0.0f) * t + (2.0f)) +
        P2 * ((-3.0f) * t3 + (4.0f) * t2 + (1.0f) * t + (0.0f)) +
        P3 * ((1.0f) * t3 + (-1.0f) * t2 + (0.0f) * t + (0.0f)));

    return result;
}

glm::vec3 findPointOnClosedCurve(const float t) {
    glm::vec3 result(0.0, 0.0, 0.0);
    int i = (int)t;
    float local_t = t - (float)i;


    const glm::vec3& P0 = controlPoints[(i - 1 + segmentsNum) % segmentsNum];
    const glm::vec3& P1 = controlPoints[i % segmentsNum];
    const glm::vec3& P2 = controlPoints[(i + 1) % segmentsNum];
    const glm::vec3& P3 = controlPoints[(i + 2) % segmentsNum];

    result = findPointOnCurveSegment(P0, P1, P2, P3, local_t);

    return result;
}

glm::vec3 find_1stDerivative_OnCurveSegment(
    const glm::vec3& P0,
    const glm::vec3& P1,
    const glm::vec3& P2,
    const glm::vec3& P3,
    const float t) {

    glm::vec3 result(1.0, 0.0, 0.0);
    const float t2 = t * t;

    result = 0.5f * (
        P0 * ((-1.0f) * 3.0f * t2 + (2.0f) * 2.0f * t + (-1.0f)) +
        P1 * ((3.0f) * 3.0f * t2 + (-5.0f) * 2.0f * t + (0.0f)) +
        P2 * ((-3.0f) * 3.0f * t2 + (4.0f) * 2.0f * t + (1.0f)) +
        P3 * ((1.0f) * 3.0f * t2 + (-1.0f) * 2.0f * t + (0.0f))
        );

    return result;
}


glm::vec3 find_1stDerivative_OnClosedCurve(const float t) {
    glm::vec3 result(1.0, 0.0, 0.0);
    size_t i = floor(t);
    float local_t = t - i;

    const glm::vec3& P0 = controlPoints[(i - 1 + segmentsNum) % segmentsNum];
    const glm::vec3& P1 = controlPoints[i % segmentsNum];
    const glm::vec3& P2 = controlPoints[(i + 1) % segmentsNum];
    const glm::vec3& P3 = controlPoints[(i + 2) % segmentsNum];

    result = find_1stDerivative_OnCurveSegment(P0, P1, P2, P3, local_t);

    return result;
}