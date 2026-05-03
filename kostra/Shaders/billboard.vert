#version 140

/* 
* shader for billboard animation
*/
in vec3 position;
in vec2 texCoord;

uniform mat4 PVMmatrix;
uniform int currentFrame;

out vec2 vTexCoord;

void main() {
    int cols = 8;
    int rows = 2;
    
    int col = currentFrame % cols;
    int row = currentFrame / cols; 
    
    float frameWidth = 1.0 / float(cols);
    float frameHeight = 1.0 / float(rows);
    
    float u = (texCoord.x * frameWidth) + (float(col) * frameWidth);
    float v = (texCoord.y * frameHeight) + (float((rows - 1) - row) * frameHeight);
    
    vTexCoord = vec2(u, v);
    
    gl_Position = PVMmatrix * vec4(position, 1.0f);
}