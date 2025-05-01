#version 460 core

// We can hardcode the grid plane directly in the vertex shader
// This is actually efficient since we're only sending 4 vertices to the GPU
const vec3 quadVertices[4] = vec3[4](
    vec3(-1.0, 0.0, -1.0), // bottom-left
    vec3( 1.0, 0.0, -1.0), // bottom-right
    vec3(-1.0, 0.0,  1.0), // top-left
    vec3( 1.0, 0.0,  1.0)  // top-right
);

// Hardcoded scaling factor - 100x100 meter grid (change as needed)
const float gridScale = 100.0;

// Combined view-projection matrix
uniform mat4 viewProjection;

out vec3 worldPos;

void main()
{
    // Get the vertex position from our hardcoded array
    vec3 pos = quadVertices[gl_VertexID] * gridScale;
    
    // Pass world position to fragment shader
    worldPos = pos;
    
    // Calculate clip space position using combined matrix
    gl_Position = viewProjection * vec4(pos, 1.0);
}