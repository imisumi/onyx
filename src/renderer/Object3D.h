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
	Object3D(std::string_view name = "New Object")
		: m_Name(name) {}
	~Object3D() = default;

	void Init(const std::vector<float> &vertices, const std::vector<uint32_t> &indices, BufferLayout layout);

	// const std::shared_ptr<VertexArray> &GetVertexArray() const { return m_VertexArray; }

	UUID GetID() const { return m_ID; }

	const std::string& GetName() const { return m_Name; }
	void SetName(const std::string &name) { m_Name = name; }

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

	glm::mat4 GetWorldTransform() const
	{
		// TODO: this is very temporary
		glm::mat4 transform = glm::mat4(1.0f);

		// Apply translation
		transform = glm::translate(transform, m_Position);

		// Apply rotation (in order: Z, Y, X)
		// transform = glm::rotate(transform, m_Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		// transform = glm::rotate(transform, m_Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		// transform = glm::rotate(transform, m_Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));

		// If m_Rotation is in degrees, convert to radians first
		transform = glm::rotate(transform, glm::radians(m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		transform = glm::rotate(transform, glm::radians(m_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		transform = glm::rotate(transform, glm::radians(m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));

		// Apply scale
		transform = glm::scale(transform, m_Scale);

		return transform;
	}

	std::shared_ptr<Mesh> GetMesh() const { return m_Mesh; }
	void SetMesh(const std::shared_ptr<Mesh> &mesh)
	{
		m_Mesh = mesh;
	}

	static std::shared_ptr<Object3D> Create(std::string_view name = "New Object")
	{
		return std::make_shared<Object3D>(name);
	}

	static std::shared_ptr<Object3D> CreateCube(std::string_view name = "New Object")
	{
		auto object = std::make_shared<Object3D>(name);
		object->SetMesh(Mesh::CreateDefaultCube());
		return object;
	}
	static std::shared_ptr<Object3D> CreateSphere(std::string_view name = "New Object")
	{
		auto object = std::make_shared<Object3D>(name);
		object->SetMesh(Mesh::CreateDefaultSphere());
		return object;
	}
	static std::shared_ptr<Object3D> CreateTriangle(std::string_view name = "New Object")
	{
		auto object = std::make_shared<Object3D>(name);
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
	std::string m_Name = "DefaultObject3D";

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