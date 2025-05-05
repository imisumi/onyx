#pragma once

#include <memory>
#include <string>

#include "renderer/Material.h"
#include "shaders/Shader.h"

#include "Mesh.h"

#include "renderer/Object3D.h"

#include "imgui/imgui.h"

#include "buffer/ShaderStorageBuffer.h"

#include "shaders/ComputeShader.h"

#include "renderer/EditorCamera.h"

class Scene
{
public:
	Scene();
	~Scene();

	void Update(float deltaTime);

	void RenderUI();

	void PrepMeshBuffers();

	void RenderViewport();
	void Render();

	uint32_t GetObjectCount() const { return static_cast<uint32_t>(m_RootObjects.size()); }

	void SetMesh(const std::shared_ptr<Mesh> &mesh) { m_CubeMesh = mesh; }
	void SetEditorCamera(const std::shared_ptr<EditorCamera> &editorCamera) { m_EditorCamera = editorCamera; }

	// inline const std::string& GetName() const { return m_DebugName; }
private:
	struct MeshInfo
	{
		glm::mat4 invModelMatrix;
		uint32_t positionOffset; // Offset into the positions array
		uint32_t normalOffset;	 // Offset into the normals array
		uint32_t indexOffset;	 // Offset into the indices array
		uint32_t numVertices;	 // Number of vertices in this mesh
		uint32_t numIndices;	 // Number of indices in this mesh
		uint32_t numFaces;		 // Number of triangles (numIndices / 3)
		uint32_t bvhNodeOffset;	 // Offset into the BVH nodes array for this mesh
		uint32_t aabbOffset;	 // Offset into the AABB array for this mesh
	};

	static_assert(sizeof(MeshInfo) == 96, "MeshInfo size mismatch!");

private:
	std::vector<MeshInfo> m_MeshInfos;

	std::string m_Name = "Default Scene";
	std::shared_ptr<MaterialLibrary> m_MaterialLibrary = std::make_shared<MaterialLibrary>();
	std::shared_ptr<ShaderLibrary> m_ShaderLibrary = std::make_shared<ShaderLibrary>();

	std::shared_ptr<Mesh> m_CubeMesh = std::make_shared<Mesh>();

	std::shared_ptr<EditorCamera> m_EditorCamera;


	std::shared_ptr<Object3D> m_SelectedObject = nullptr;
	std::vector<std::shared_ptr<Object3D>> m_RootObjects;

	ShaderStorageBuffer m_AABBBuffer;
	uint32_t m_AABBBufferCount = 0;
	ShaderStorageBuffer m_PositionsBuffer;
	ShaderStorageBuffer m_NormalsBuffer;
	ShaderStorageBuffer m_IndicesBuffer;
	ShaderStorageBuffer m_OutputAABB;
	ShaderStorageBuffer m_ReccursiveAABB;
	ShaderStorageBuffer m_OffsetBuffer;

	std::shared_ptr<Shader> m_Shader;
	std::shared_ptr<ComputeShader> m_ComputeShader = std::make_shared<ComputeShader>();
	std::shared_ptr<ComputeShader> m_ComputeCalcMinMax = std::make_shared<ComputeShader>();


	bool m_VisualizeBLAS = true;
};