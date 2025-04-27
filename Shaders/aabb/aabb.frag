#version 460 core
// Input from vertex shader
in vec3 fragColor;

// Output color
out vec4 outColor;

void main() {
    outColor = vec4(fragColor, 1.0);
}