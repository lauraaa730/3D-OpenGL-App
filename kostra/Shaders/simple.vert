#version 140

in vec3 position;
in vec2 texCoord;
in vec3 color;

uniform mat4 PVM;
uniform bool hasTexture;

out vec2 vTexCoord;
out vec3 vColor;

void main() {
	if (hasTexture) {
		vTexCoord = texCoord;
	} else {
	vColor = color;
	}
	
	gl_Position = PVM * vec4(position, 1.0f);
}