#pragma once

#include <cstdint>
#include <vector>

class BLASManager
{
public:
	struct MeshInfo
	{
		uint32_t positionOffset; // Offset into the positions array
		uint32_t normalOffset;	 // Offset into the normals array
		uint32_t indexOffset;	 // Offset into the indices array
		uint32_t numVertices;	 // Number of vertices in this mesh
		uint32_t numIndices;	 // Number of indices in this mesh
		uint32_t numFaces;		 // Number of triangles (numIndices / 3)
		uint32_t bvhNodeOffset;	 // Offset into the BVH nodes array for this mesh
		uint32_t aabbOffset;	 // Offset into the AABB array for this mesh
	};

private:
	std::vector<MeshInfo> meshInfos;
	uint32_t positionOffsetCounter = 0;
	uint32_t normalOffsetCounter = 0;
	uint32_t indexOffsetCounter = 0;
	uint32_t bvhNodeOffsetCounter = 0; // Assuming you have a counter for BVH nodes
	uint32_t aabbOffsetCounter = 0;	   // Assuming you have a counter for AABBs
};