#version 140

in vec2 vTexCoord;
in vec3 vColor;

uniform sampler2D texSampler;
uniform bool hasTexture;

out vec4 fragmentColor;

void main() {
	if (hasTexture) {
		fragmentColor = texture(texSampler, vTexCoord);
	} else {
		fragmentColor = vec4(vColor, 1.0f);
	}
}