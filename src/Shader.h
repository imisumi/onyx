#pragma once

#include <string>
#include <unordered_map>
#include <memory>

#include <glm/glm.hpp>


#include "ShaderUniformSetter.h"

class Shader : public ShaderUniformSetter<Shader>
{
public:
	Shader(const std::string &filepath);
	Shader(const std::string &name, const std::string &vertexSrc, const std::string &fragmentSrc);
	Shader(const std::string &name, const std::string &vertexSrc, const std::string &fragmentSrc, const std::string &geometrySrc);
	~Shader();

	void SetName(const std::string &name) { m_Name = name; }
	const std::string &GetName() { return m_Name; }

	void Bind() const;
	void Unbind() const;

	// uint32_t GetRendererID() const { return m_RendererID; }
	uint32_t GetProgramID() const { return m_RendererID; }

	static std::shared_ptr<Shader> Create(const std::string &filepath);
	static std::shared_ptr<Shader> Create(const std::string &name, const std::string &vertexSrc, const std::string &fragmentSrc);
	static std::shared_ptr<Shader> Create(const std::string &name, const std::string &vertexSrc, const std::string &fragmentSrc, const std::string &geometrySrc);

private:
	std::string ReadFile(const std::string &filepath);
	void CreateProgram(uint32_t vertexShader, uint32_t fragmentShader);
	void CreateProgram(uint32_t vertexShader, uint32_t fragmentShader, uint32_t geometryShader);
	uint32_t CompileShader(uint32_t type, const std::string &source);
	std::pair<std::string, std::string> extractVersionDirective(const std::string &shaderCode);

private:
	uint32_t m_RendererID;
	std::string m_Name;

	std::unordered_map<std::string, uint32_t> m_UniformLocationCache;
};

class ShaderLibrary
{
public:
	void Add(const std::shared_ptr<Shader> &shader);
	void Add(const std::string &name, const std::shared_ptr<Shader> &shader);
	std::shared_ptr<Shader> Load(const std::string &filepath);
	std::shared_ptr<Shader> Load(const std::string &name, const std::string &filepath);

	std::shared_ptr<Shader> Get(const std::string &name);
	bool Exists(const std::string &name) const;



	typedef typename std::unordered_map<std::string, std::shared_ptr<Shader>>::iterator iterator;
	typedef typename std::unordered_map<std::string, std::shared_ptr<Shader>>::const_iterator const_iterator;

	// Iterator functions
	iterator begin() { return m_Shaders.begin(); }
	iterator end() { return m_Shaders.end(); }
	const_iterator begin() const { return m_Shaders.begin(); }
	const_iterator end() const { return m_Shaders.end(); }
private:
	std::unordered_map<std::string, std::shared_ptr<Shader>> m_Shaders;
};
