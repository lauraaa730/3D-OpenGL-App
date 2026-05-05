#version 140

in vec2 vTexCoord;
in float distanceFromCamera;

uniform sampler2D texSampler;

out vec4 fragmentColor;

void main() {
    vec3 fogColor = vec3(0.0, 0.1953, 0.5039);
    float fogStart = 5.0;
    float fogEnd = 10.0;
    float fogFactor = (fogEnd - distanceFromCamera) / (fogEnd - fogStart);

    vec4 texColor = texture(texSampler, vTexCoord);

    vec3 finalColor = mix(fogColor, texColor.xyz, fogFactor);

    fragmentColor = vec4(finalColor, texColor.a);
}