#pragma once
#include "pgr.h"

extern glm::vec3 controlPoints[];
extern int segmentsNum;


glm::vec3 findPointOnCurveSegment(
    const glm::vec3& P0,
    const glm::vec3& P1,
    const glm::vec3& P2,
    const glm::vec3& P3,
    const float t
);


glm::vec3 findPointOnClosedCurve(
    const float t
);

glm::vec3 find_1stDerivative_OnCurveSegment(
    const glm::vec3& P0,
    const glm::vec3& P1,
    const glm::vec3& P2,
    const glm::vec3& P3,
    const float t
);


glm::vec3 find_1stDerivative_OnClosedCurve(
    const float t
);


