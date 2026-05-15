/**
 * \file    billboard.frag
 * \author  Laura Katerina Dudkova
 * \date    2026/05/11
 * \brief   Fragment shader for animated billboards with distance-based fog blending.
 */
#version 140

in vec2 vTexCoord;           ///< \brief Interpolated texture coordinates from the vertex shader.
in float distanceFromCamera; ///< \brief Calculated distance from the camera for fog application.

uniform sampler2D texSampler; ///< \brief Sprite sheet texture sampler.

out vec4 fragmentColor;

/**
 * \brief Main fragment shader entry point.
 * \details Samples the sprite sheet and applies a linear distance fog effect.
 */
void main() {
    vec3 fogColor = vec3(0.0, 0.1953, 0.5039);
    float fogStart = 5.0;
    float fogEnd = 10.0;
    
    //calculate linear fog factor based on distance. 
    //1.0 means no fog (at fogStart), 0.0 means full fog (at fogEnd).
    float fogFactor = (fogEnd - distanceFromCamera) / (fogEnd - fogStart);

    vec4 texColor = texture(texSampler, vTexCoord);

    //linearly interpolate between the fog color and the texture color using the calculated factor.
    vec3 finalColor = mix(fogColor, texColor.xyz, fogFactor);

    //preserve the original texture alpha to keep the billboard transparency intact
    fragmentColor = vec4(finalColor, texColor.a);
}