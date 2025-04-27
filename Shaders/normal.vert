#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 viewProj;
uniform mat4 model;

out vec3 fragColor;

void main()
{
	// fragColor = normalize(aNormal) * 0.5 + 0.5;
	fragColor = vec3(model * vec4(aPos, 1.0));  // Calculate world position
    gl_Position = viewProj * model * vec4(aPos, 1.0);
}