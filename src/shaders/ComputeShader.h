#pragma once

#include <GL/glew.h>
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

#include "shaders/ShaderUniformSetter.h"

class ComputeShader : public ShaderUniformSetter<ComputeShader>
{

public:
	ComputeShader();
	ComputeShader(const std::string &filePath);
	~ComputeShader();

	const std::string &GetName() const { return m_Name; }
	void SetName(const std::string &name) { m_Name = name; }

	// Core functionality
	bool LoadFromFile(const std::string &filePath);
	bool LoadFromString(const std::string &shaderSource);
	void Use();
	void Dispatch(uint32_t numGroupsX, uint32_t numGroupsY = 1, uint32_t numGroupsZ = 1);
	void Wait(); // Inserts a memory barrier to wait for completion

	// Getters
	uint32_t GetProgramID() const { return m_ProgramID; }

	static std::shared_ptr<ComputeShader> Create(const std::string &filePath)
	{
		return std::make_shared<ComputeShader>(filePath);
	}

private:
	std::string m_Name;
	uint32_t m_ProgramID;
	uint32_t m_ShaderID;

	// Helper functions
	bool CompileShader(const std::string &source);
	bool LinkProgram();
};
