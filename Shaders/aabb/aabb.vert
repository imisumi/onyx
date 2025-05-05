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

struct MeshInfo
{
	mat4 invModelMatrix;
    uint positionOffset; // Offset into the positions array
    uint normalOffset;   // Offset into the normals array
    uint indexOffset;    // Offset into the indices array
    uint numVertices;    // Number of vertices in this mesh
    uint numIndices;     // Number of indices in this mesh
    uint numFaces;       // Number of triangles (numIndices / 3)
    uint bvhNodeOffset;  // Offset into the BVH nodes array for this mesh
    uint aabbOffset;     // Offset into the AABB array for this mesh
};

layout(std430, binding = 30) buffer meshInfo {
    MeshInfo meshes[];
};

uniform mat4 viewProj;
uniform uint meshIndex; // New uniform to specify which mesh we're rendering

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
    // Get the current mesh info
    MeshInfo mesh = meshes[meshIndex];
    
    // Calculate the actual node index including the mesh's BVH offset
    int nodeIndex = int(mesh.bvhNodeOffset) + gl_InstanceID;
    
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
    fragColor = neonColors[gl_InstanceID % 8];
}