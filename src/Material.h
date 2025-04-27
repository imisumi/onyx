#pragma once

#include <string>
#include <unordered_map>
#include <memory>

#include "Shader.h"

#include <glm/glm.hpp>

class Material
{
public:
	Material() = default;
	~Material() = default;

	Material(const std::shared_ptr<Shader> &shader)
		: m_Shader(shader), m_Name(shader->GetName()) {}

	void SetShader(const std::shared_ptr<Shader> &shader) { m_Shader = shader; }
	const std::shared_ptr<Shader> &GetShader() const { return m_Shader; }

	void SetColor(const glm::vec4 &color) { m_Color = color; }
	const glm::vec4 &GetColor() const { return m_Color; }

	void SetName(const std::string &name) { m_Name = name; }
	const std::string &GetName() const { return m_Name; }

	void Use() const;

	static std::shared_ptr<Material> Create(const std::shared_ptr<Shader> &shader)
	{
		return std::make_shared<Material>(shader);
	}

private:
	std::string m_Name = "DefaultMaterial";
	std::shared_ptr<Shader> m_Shader;
	glm::vec4 m_Color = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f); // Default color (white)
};

class MaterialLibrary
{
public:
	MaterialLibrary() = default;
	~MaterialLibrary() = default;

	void AddMaterial(const std::shared_ptr<Material> &material)
	{
		if (m_Materials.contains(material->GetName()))
		{
			MY_ASSERT(false, "Material already exists!");
		}
		m_Materials[material->GetName()] = material;
	}

	std::shared_ptr<Material> GetMaterial(const std::string &name)
	{
		auto it = m_Materials.find(name);
		if (it != m_Materials.end())
		{
			return it->second;
		}
		MY_ASSERT(false, "Material not found!");
		return nullptr; // or throw an exception if preferred
						// return nullptr; // or throw an exception if preferred
	}

	typedef typename std::unordered_map<std::string, std::shared_ptr<Material>>::iterator iterator;
	typedef typename std::unordered_map<std::string, std::shared_ptr<Material>>::const_iterator const_iterator;

	// Iterator functions
	iterator begin() { return m_Materials.begin(); }
	iterator end() { return m_Materials.end(); }
	const_iterator begin() const { return m_Materials.begin(); }
	const_iterator end() const { return m_Materials.end(); }

private:
	std::unordered_map<std::string, std::shared_ptr<Material>> m_Materials;
};