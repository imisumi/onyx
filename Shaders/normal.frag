// #version 460 core
// out vec4 FragColor;

// // uniform vec3 triangleColor;

// in vec3 fragColor;

// void main()
// {
//     // FragColor = vec4(triangleColor, 1.0);

// 	vec3 flatNormal = normalize(cross(dFdx(fragColor), dFdy(fragColor)));
// 	vec3 normalColor = flatNormal * 0.5 + 0.5;
// 	FragColor = vec4(normalColor, 1.0f);
// 	// FragColor = vec4(fragColor, 1.0f);
// }

#version 460 core
out vec4 FragColor;

in vec3 fragNormal;

void main()
{
    // Convert normal to color space (from [-1,1] to [0,1] range)
    vec3 normalColor = fragNormal * 0.5 + 0.5;
    
    FragColor = vec4(normalColor, 1.0f);
}