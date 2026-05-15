//----------------------------------------------------------------------------------------
/**
 * \file    Spline.h
 * \author  Laura Katerina Dudkova
 * \date    2026/05/11
 * \brief   Declarations for spline curve evaluation and its derivatives.
 */

#pragma once
#include "pgr.h"

extern glm::vec3 controlPoints[]; ///< \brief Array of control points defining the curve shape.
extern int segmentsNum;           ///< \brief Number of segments in the defined spline.
extern float curveScale;          ///< \brief Scaling factor applied to the final calculated curve points.

/**
 * \brief Calculates a position on a specific spline segment using Catmull-Rom interpolation.
 * \param P0 First control point.
 * \param P1 Second control point (start of the segment).
 * \param P2 Third control point (end of the segment).
 * \param P3 Fourth control point.
 * \param t Local parameter within the segment [0.0, 1.0].
 * \return Interpolated 3D position on the segment.
 */
glm::vec3 findPointOnCurveSegment(
    const glm::vec3& P0,
    const glm::vec3& P1,
    const glm::vec3& P2,
    const glm::vec3& P3,
    const float t
);

/**
 * \brief Calculates a position on the continuous closed spline curve.
 * \param t Global curve parameter determining the position along the entire closed loop.
 * \return Interpolated 3D position on the closed curve.
 */
glm::vec3 findPointOnClosedCurve(
    const float t
);

/**
 * \brief Calculates the tangent (first derivative) on a specific spline segment.
 * \param P0 First control point.
 * \param P1 Second control point.
 * \param P2 Third control point.
 * \param P3 Fourth control point.
 * \param t Local parameter within the segment [0.0, 1.0].
 * \return 3D direction vector representing the tangent at parameter t.
 */
glm::vec3 find_1stDerivative_OnCurveSegment(
    const glm::vec3& P0,
    const glm::vec3& P1,
    const glm::vec3& P2,
    const glm::vec3& P3,
    const float t
);

/**
 * \brief Calculates the tangent (first derivative) on the continuous closed curve.
 * \param t Global curve parameter.
 * \return 3D direction vector representing the tangent on the closed curve.
 */
glm::vec3 find_1stDerivative_OnClosedCurve(
    const float t
);