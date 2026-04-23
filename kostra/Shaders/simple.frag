#version 140

in vec2 vTexCoord;

uniform sampler2D texSampler;

out vec4 fragmentColor;

void main() {
	fragmentColor = texture(texSampler, vTexCoord);
}