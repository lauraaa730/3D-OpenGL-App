#version 140

in vec3 position;
in vec3 normal;
in vec2 texCoord;
in vec3 color;

uniform int currentFrame;
uniform bool hasTexture;

uniform mat4 PVMmatrix;
uniform mat4 Vmatrix;
uniform mat4 Mmatrix;
uniform mat3 normalMatrix;

out vec3 vPos;
out vec3 vNormal;
out vec2 vTexCoord;
out vec3 vColor;
out float distanceFromCamera;

void main() {
	vec4 worldPos = Mmatrix * vec4(position, 1.0);
    vPos = (Vmatrix * worldPos).xyz;

	vNormal = normalize(mat3(Vmatrix) * normalMatrix * normal);

	if (hasTexture) {
		vTexCoord = texCoord;
	} else {
		vColor = color;
	}
	
	distanceFromCamera = length(-vPos);
	gl_Position = PVMmatrix * vec4(position, 1.0f);
}