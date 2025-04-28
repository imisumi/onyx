#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <vector>

#include "Mesh.h"

#include "core/UUID.h"

#include <glm/glm.hpp>

#include "Material.h"

#include "Scene.h"

#include <filesystem>

class Object3D
{
public:
	Object3D() = default;
	~Object3D() = default;

	void Init(const std::vector<float> &vertices, const std::vector<uint32_t> &indices, BufferLayout layout);

	// const std::shared_ptr<VertexArray> &GetVertexArray() const { return m_VertexArray; }

	UUID GetID() const { return m_ID; }

	glm::vec3 GetPosition() const { return m_Position; }
	void SetPosition(const glm::vec3 &position)
	{
		m_Position = position;
		m_TransformDirty = true;
	}

	glm::vec3 GetRotation() const { return m_Rotation; }
	void SetRotation(const glm::vec3 &rotation)
	{
		m_Rotation = rotation;
		m_TransformDirty = true;
	}

	glm::vec3 GetScale() const { return m_Scale; }
	void SetScale(const glm::vec3 &scale)
	{
		m_Scale = scale;
		m_TransformDirty = true;
	}

	std::shared_ptr<Mesh> GetMesh() const { return m_Mesh; }
	void SetMesh(const std::shared_ptr<Mesh> &mesh)
	{
		m_Mesh = mesh;
	}

	static std::shared_ptr<Object3D> Create(const std::string &name = "DefaultObject3D")
	{
		return std::make_shared<Object3D>();
	}

	static std::shared_ptr<Object3D> CreateCube(const std::string &name = "DefaultObject3D")
	{
		auto object = std::make_shared<Object3D>();
		object->SetMesh(Mesh::CreateDefaultCube());
		return object;
	}
	static std::shared_ptr<Object3D> CreateSphere(const std::string &name = "DefaultObject3D")
	{
		auto object = std::make_shared<Object3D>();
		object->SetMesh(Mesh::CreateDefaultSphere());
		return object;
	}
	static std::shared_ptr<Object3D> CreateTriangle(const std::string &name = "DefaultObject3D")
	{
		auto object = std::make_shared<Object3D>();
		object->SetMesh(Mesh::CreateDefaultTriangle());
		return object;
	}

	static std::shared_ptr<Object3D> CreateMeshFromFile(const std::filesystem::path &file)
	{
		auto object = std::make_shared<Object3D>();
		object->SetMesh(Mesh::LoadFromFile(file));
		return object;
	}

	std::shared_ptr<Material> GetMaterial() const { return m_Material; }
	void SetMaterial(const std::shared_ptr<Material> &material)
	{
		m_Material = material;
	}

private:
	//? Identity
	UUID m_ID;

	//? Transform
	glm::vec3 m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 m_Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 m_Scale = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::mat4 m_LocalTransform = glm::mat4(1.0f); //? Local transform for the object
	glm::mat4 m_WorldTransform = glm::mat4(1.0f); //? Parent transform + local transform

	bool m_TransformDirty = true; //? Meaning we need to revalidate the transform matrix

	//? Appearance
	//? for camera, light it will be a way to visualize them in the viewport
	std::shared_ptr<Mesh> m_Mesh = Mesh::CreateDefaultSphere();
	// std::vector<std::shared_ptr<Material>> m_Materials;
	std::shared_ptr<Material> m_Material = std::make_shared<Material>();
	bool m_Visible;

	//? Hierarchy
	std::weak_ptr<Object3D> m_Parent;
	std::vector<std::shared_ptr<Object3D>> m_Children;
	// std::weak_ptr<Scene> m_Scene;

};

// class Camera : public Object3D
// {
// };