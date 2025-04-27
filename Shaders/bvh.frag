#version 460 core

// Input color from geometry shader
in vec4 color;

// Output color
out vec4 fragColor;

layout(std430, binding = 1) buffer tempBuffer {
    float f[];
};

void main() {
    // Pass through the color set in the geometry shader
    fragColor = color;
    // fragColor = vec4(f[0], 0.0, 0.0, 1.0);
}