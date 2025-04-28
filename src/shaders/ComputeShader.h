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
	~ComputeShader();

	// Core functionality
	bool LoadFromFile(const std::string &filePath);
	bool LoadFromString(const std::string &shaderSource);
	void Use();
	void Dispatch(uint32_t numGroupsX, uint32_t numGroupsY = 1, uint32_t numGroupsZ = 1);
	void Wait(); // Inserts a memory barrier to wait for completion

	// Getters
	uint32_t GetProgramID() const { return m_ProgramID; }

private:
	uint32_t m_ProgramID;
	uint32_t m_ShaderID;

	// Helper functions
	bool CompileShader(const std::string &source);
	bool LinkProgram();
};
