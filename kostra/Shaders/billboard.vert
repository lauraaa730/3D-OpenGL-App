/**
 * \file    billboard.vert
 * \author  Laura Katerina Dudkova
 * \date    2026/05/11
 * \brief   Vertex shader for animated billboards handling sprite sheet UV offsets and fog distance.
 */
#version 140

in vec3 position;
in vec2 texCoord;

uniform mat4 PVMmatrix;     ///< \brief Projection * View * Model matrix.
uniform int currentFrame;   ///< \brief Current animation frame index.

out vec2 vTexCoord;
out float distanceFromCamera;

/**
 * \brief Main vertex shader entry point.
 * \details Calculates the correct UV offsets for the current frame on a 2D sprite sheet 
 *          and computes the distance to the camera for the fragment shader fog.
 */
void main() {
    vec4 vPos = PVMmatrix * vec4(position, 1.0f);

    // --- Sprite Sheet Texture Mapping ---
    int cols = 8;
    int rows = 2;
    
    //calculate the 2D grid position (col, row) from the 1D currentFrame index
    int col = currentFrame % cols;
    int row = currentFrame / cols; 
    
    //calculate the UV dimensions of a single frame
    float frameWidth = 1.0 / float(cols);
    float frameHeight = 1.0 / float(rows);
    
    //offset the base texture coordinates to point to the correct frame cell.
    //the row is inverted ((rows - 1) - row) because OpenGL's texture V-axis starts at the bottom-left.
    float u = (texCoord.x * frameWidth) + (float(col) * frameWidth);
    float v = (texCoord.y * frameHeight) + (float((rows - 1) - row) * frameHeight);
    
    vTexCoord = vec2(u, v);

    // --- Fog Distance Calculation ---
    //length of the vector between camera and the object in view space is -vPos
    distanceFromCamera = length(-vPos.xyz);
    
    gl_Position = PVMmatrix * vec4(position, 1.0f);
}