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
	virtual ~Scene();

	// virtual void OnAttach() {}
	// virtual void OnDetach() {}
	// virtual void OnUpdate(float deltaTime) {}
	// virtual void OnImGuiRender() {}
	// virtual void OnEvent(Event& event) {}

	void Update(float deltaTime);

	void OnImGuiRender();

	void Render();

	void SetMesh(const std::shared_ptr<Mesh> &mesh) { m_CubeMesh = mesh; }
	void SetEditorCamera(const std::shared_ptr<EditorCamera> &editorCamera) { m_EditorCamera = editorCamera; }

	// inline const std::string& GetName() const { return m_DebugName; }
private:
	std::string m_Name = "Default Scene";
	std::shared_ptr<MaterialLibrary> m_MaterialLibrary = std::make_shared<MaterialLibrary>();
	std::shared_ptr<ShaderLibrary> m_ShaderLibrary = std::make_shared<ShaderLibrary>();

	std::shared_ptr<Mesh> m_CubeMesh = std::make_shared<Mesh>();

	std::shared_ptr<EditorCamera> m_EditorCamera;

	std::vector<std::shared_ptr<Object3D>> m_RootObjects;
	// std::unordered_map<uint32_t, std::weak_ptr<Object3D>> m_ObjectsById;
	// std::unordered_map<std::string, std::weak_ptr<Object3D>> m_ObjectsByName;

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
};