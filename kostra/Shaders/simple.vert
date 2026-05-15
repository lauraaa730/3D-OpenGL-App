/**
 * \file    simple.vert
 * \author  Laura Katerina Dudkova
 * \date    2026/05/11
 * \brief   Main vertex shader for standard 3D objects, handling transformations and texture coordinates.
 */
#version 140

in vec3 position; ///< \brief Object-space vertex position.
in vec3 normal;   ///< \brief Object-space vertex normal.
in vec2 texCoord; ///< \brief Vertex texture coordinates.
in vec3 color;    ///< \brief Vertex color (used if texturing is disabled).

uniform int currentFrame; ///< \brief Current animation frame (if applicable).
uniform bool hasTexture;  ///< \brief Flag indicating whether the object uses a texture.

uniform mat4 PVMmatrix;   ///< \brief Projection * View * Model matrix.
uniform mat4 Vmatrix;     ///< \brief View matrix.
uniform mat4 Mmatrix;     ///< \brief Model matrix.
uniform mat3 normalMatrix;///< \brief Matrix for transforming normals to view space.
uniform mat4 UVMatrix;    ///< \brief Matrix for transforming texture coordinates.

out vec3 vPos;               ///< \brief Interpolated view-space position.
out vec3 vNormal;            ///< \brief Interpolated view-space normal.
out vec2 vTexCoord;          ///< \brief Interpolated texture coordinates.
out vec3 vColor;             ///< \brief Interpolated vertex color.
out float distanceFromCamera;///< \brief Distance from the camera for fog calculations.

/**
 * \brief Main vertex shader entry point.
 * \details Calculates view-space positions, transforms normals, and prepares data for the fragment shader.
 */
void main() {
	//calculate world position and view position
	vec4 worldPos = Mmatrix * vec4(position, 1.0);
    vPos = (Vmatrix * worldPos).xyz;

	//transform normal to view space using the precalculated normal matrix
	vNormal = normalize(mat3(Vmatrix) * normalMatrix * normal);

	//apply texture coordinate transformations if texture exists, otherwise pass color
	if (hasTexture) {
		vTexCoord = (UVMatrix * vec4(texCoord, 0.0, 1.0)).xy;
	} else {
		vColor = color;
	}
	
	//calculate exact distance from camera for exponential fog
	distanceFromCamera = length(-vPos);
	
	gl_Position = PVMmatrix * vec4(position, 1.0f);
}