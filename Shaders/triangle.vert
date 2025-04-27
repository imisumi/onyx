#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

uniform mat4 viewProj;
uniform mat4 model;

out vec4 fragColor;

void main()
{
	fragColor = aColor;
    gl_Position = viewProj * model * vec4(aPos, 1.0);
}