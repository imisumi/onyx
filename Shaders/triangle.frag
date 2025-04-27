#version 460 core
out vec4 FragColor;

// uniform vec3 triangleColor;

in vec4 fragColor;

void main()
{
    // FragColor = vec4(triangleColor, 1.0);
	FragColor = fragColor;
}