#version 140

in vec3 position;
in vec2 texCoord;

uniform mat4 PVM;

out vec2 vTexCoord;

void main() {
	vTexCoord = texCoord;
	gl_Position = PVM * vec4(position, 1.0f);
}