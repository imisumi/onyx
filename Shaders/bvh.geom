#version 460 core

// Input/output definitions
layout(points) in;
layout(line_strip, max_vertices = 128) out;

// Camera matrix
uniform mat4 viewProj;

// Buffer containing BVH nodes
// layout(std430, binding = 0) buffer BVHBuffer {
//     // Format: [minX, minY, minZ, maxX, maxY, maxZ, leftChildOrPrimOffset, rightChildOrPrimCount]
//     float bvhData[];
// };

struct BVHNode {
        float minX, minY, minZ;    // 12 bytes - bounding box min point
        float maxX, maxY, maxZ;    // 12 bytes - bounding box max point
        int leftChildOrPrimOffset;  // 4 bytes - child node index or triangleId offset if negative
        int rightChildOrPrimCount;  // 4 bytes - right child index or primitive count when leftChildOrPrimOffset < 0
    };

layout(std430, binding = 0) buffer BVHBuffer {
    BVHNode nodes[];
};

layout(std430, binding = 1) buffer tempBuffer {
    float f[];
};

// Root node index to start traversal
uniform int rootNodeIndex = 0;

// Simple color output
out vec4 color;

void addAABB(vec3 minPoint, vec3 maxPoint)
{
	vec3 corners[8];
    corners[0] = vec3(minPoint.x, minPoint.y, minPoint.z);
    corners[1] = vec3(maxPoint.x, minPoint.y, minPoint.z);
    corners[2] = vec3(maxPoint.x, maxPoint.y, minPoint.z);
    corners[3] = vec3(minPoint.x, maxPoint.y, minPoint.z);
    corners[4] = vec3(minPoint.x, minPoint.y, maxPoint.z);
    corners[5] = vec3(maxPoint.x, minPoint.y, maxPoint.z);
    corners[6] = vec3(maxPoint.x, maxPoint.y, maxPoint.z);
    corners[7] = vec3(minPoint.x, maxPoint.y, maxPoint.z);
    
    // Bottom face
    gl_Position = viewProj * vec4(corners[0], 1.0);
    EmitVertex();
    gl_Position = viewProj * vec4(corners[1], 1.0);
    EmitVertex();
    EndPrimitive();
    
    gl_Position = viewProj * vec4(corners[1], 1.0);
    EmitVertex();
    gl_Position = viewProj * vec4(corners[2], 1.0);
    EmitVertex();
    EndPrimitive();
    
    gl_Position = viewProj * vec4(corners[2], 1.0);
    EmitVertex();
    gl_Position = viewProj * vec4(corners[3], 1.0);
    EmitVertex();
    EndPrimitive();
    
    gl_Position = viewProj * vec4(corners[3], 1.0);
    EmitVertex();
    gl_Position = viewProj * vec4(corners[0], 1.0);
    EmitVertex();
    EndPrimitive();
    
    // Top face
    gl_Position = viewProj * vec4(corners[4], 1.0);
    EmitVertex();
    gl_Position = viewProj * vec4(corners[5], 1.0);
    EmitVertex();
    EndPrimitive();
    
    gl_Position = viewProj * vec4(corners[5], 1.0);
    EmitVertex();
    gl_Position = viewProj * vec4(corners[6], 1.0);
    EmitVertex();
    EndPrimitive();
    
    gl_Position = viewProj * vec4(corners[6], 1.0);
    EmitVertex();
    gl_Position = viewProj * vec4(corners[7], 1.0);
    EmitVertex();
    EndPrimitive();
    
    gl_Position = viewProj * vec4(corners[7], 1.0);
    EmitVertex();
    gl_Position = viewProj * vec4(corners[4], 1.0);
    EmitVertex();
    EndPrimitive();
    
    // Side edges
    gl_Position = viewProj * vec4(corners[0], 1.0);
    EmitVertex();
    gl_Position = viewProj * vec4(corners[4], 1.0);
    EmitVertex();
    EndPrimitive();
    
    gl_Position = viewProj * vec4(corners[1], 1.0);
    EmitVertex();
    gl_Position = viewProj * vec4(corners[5], 1.0);
    EmitVertex();
    EndPrimitive();
    
    gl_Position = viewProj * vec4(corners[2], 1.0);
    EmitVertex();
    gl_Position = viewProj * vec4(corners[6], 1.0);
    EmitVertex();
    EndPrimitive();
    
    gl_Position = viewProj * vec4(corners[3], 1.0);
    EmitVertex();
    gl_Position = viewProj * vec4(corners[7], 1.0);
    EmitVertex();
    EndPrimitive();
}

void main() {
    color = vec4(0.0, 1.0, 0.0, 1.0);
	float x = f[0];

    int indexStack[16];
    int currentIndex = 0;
    indexStack[currentIndex++] = rootNodeIndex;

    int emittedVertices = 0;
    const int MAX_VERTICES = 120; // Leave some margin

    while (currentIndex > 0 && emittedVertices < MAX_VERTICES) {
		int nodeIndex = indexStack[--currentIndex];
		BVHNode node = nodes[nodeIndex];

		vec3 minPoint = vec3(node.minX, node.minY, node.minZ);
		vec3 maxPoint = vec3(node.maxX, node.maxY, node.maxZ);
		int leftIndex = node.leftChildOrPrimOffset;
		int rightIndex = node.rightChildOrPrimCount;
        
        // Each call to addAABB adds 24 vertices (12 lines × 2 vertices each)
        if (emittedVertices + 24 <= MAX_VERTICES) {
            addAABB(minPoint, maxPoint);
            emittedVertices += 24;
            
            // Only push children if we have capacity to render them
            if (leftIndex >= 0) {
                indexStack[currentIndex++] = leftIndex;
            }
            if (rightIndex >= 0) {
                indexStack[currentIndex++] = rightIndex;
            }
        }
    }
}
