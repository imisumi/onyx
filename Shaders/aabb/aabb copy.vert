// #version 460 core

// // Input vertex data
// layout(location = 0) in vec3 localVertex;

// // SSBO containing all bounding boxes data - stored as consecutive floats
// layout(std430, binding = 23) buffer BoundingBoxes {
//     float boxes[];  // Flat array of floats: minx, miny, minz, maxx, maxy, maxz, ...
// };

// uniform mat4 viewProj;
// // uniform mat4 model;

// out vec3 fragColor;

// void main() {
//     // Get the instance (box) index
//     int boxIndex = gl_InstanceID;
    
//     // Calculate offset in SSBO for this box (6 floats per box)
//     int dataOffset = boxIndex * 6;
    
//     // Extract min/max points from SSBO - directly access the float array
//     float minX = boxes[dataOffset];
//     float minY = boxes[dataOffset + 1];
//     float minZ = boxes[dataOffset + 2];
//     float maxX = boxes[dataOffset + 3];
//     float maxY = boxes[dataOffset + 4];
//     float maxZ = boxes[dataOffset + 5];
    
//     // Scale and translate the local vertex
//     vec3 size = vec3(maxX - minX, maxY - minY, maxZ - minZ);
//     // vec3 position = localVertex * size + vec3(minX, minY, minZ);
// 	// vec3 position = size + vec3(minX, minY, minZ);
// 	vec3 position = (localVertex * 0.5 + 0.5) * size + vec3(minX, minY, minZ);
    
//     // Output the final position
//     // gl_Position = viewProj * model * vec4(position, 1.0);
//     gl_Position = viewProj * vec4(position, 1.0);
    
//     // Set fragment color
//     fragColor = vec3(0.0, 0.8, 1.0);
// }

#version 460 core

// Input vertex data
layout(location = 0) in vec3 localVertex;

struct BVHNode
{
	float minX, minY, minZ;    // 12 bytes - bounding box min point
	float maxX, maxY, maxZ;    // 12 bytes - bounding box max point
	int leftChildOrPrimOffset;  // 4 bytes - child node index or triangleId offset if negative
	int rightChildOrPrimCount;  // 4 bytes - right child index or primitive count when leftChildOrPrimOffset < 0
};

layout(std430, binding = 24) buffer reccAABB {
    BVHNode nodes[];
};

uniform mat4 viewProj;
// uniform mat4 model;

out vec3 fragColor;

// Define a struct of neon colors
const vec3 neonColors[8] = vec3[8](
    vec3(1.0, 0.2, 0.8),  // Neon pink
    vec3(0.0, 1.0, 0.8),  // Neon cyan
    vec3(0.8, 1.0, 0.0),  // Neon yellow
    vec3(0.8, 0.0, 1.0),  // Neon purple
    vec3(0.0, 0.8, 1.0),  // Neon blue
    vec3(1.0, 0.8, 0.0),  // Neon orange
    vec3(0.0, 1.0, 0.4),  // Neon green
    vec3(1.0, 0.4, 0.0)   // Neon red-orange
);

void main() {
    // Get the instance (node) index
    int nodeIndex = gl_InstanceID;
    
    // Extract min/max points directly from BVHNode struct
    float minX = nodes[nodeIndex].minX;
    float minY = nodes[nodeIndex].minY;
    float minZ = nodes[nodeIndex].minZ;
    float maxX = nodes[nodeIndex].maxX;
    float maxY = nodes[nodeIndex].maxY;
    float maxZ = nodes[nodeIndex].maxZ;
    
    // Scale and translate the local vertex
    vec3 size = vec3(maxX - minX, maxY - minY, maxZ - minZ);
    vec3 position = (localVertex * 0.5 + 0.5) * size + vec3(minX, minY, minZ);
    
    // Output the final position
    gl_Position = viewProj * vec4(position, 1.0);
    
    // Set fragment color from the neon color palette based on instance ID
    fragColor = neonColors[nodeIndex % 8];
}