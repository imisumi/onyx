#include "ComputeShader.h"

#include <GL/glew.h>

#include <iostream>
#include <fstream>
#include <sstream>

ComputeShader::ComputeShader()
	: m_ProgramID(0), m_ShaderID(0)
{
}

ComputeShader::ComputeShader(const std::string &filePath)
	: m_ProgramID(0), m_ShaderID(0), m_Name(filePath)
{
	LoadFromFile(filePath);
}

ComputeShader::~ComputeShader()
{
	if (m_ProgramID != 0)
	{
		glDeleteProgram(m_ProgramID);
	}
	if (m_ShaderID != 0)
	{
		glDeleteShader(m_ShaderID);
	}
}

bool ComputeShader::CompileShader(const std::string &source)
{
	m_ShaderID = glCreateShader(GL_COMPUTE_SHADER);

	const char *sourcePtr = source.c_str();
	glShaderSource(m_ShaderID, 1, &sourcePtr, nullptr);
	glCompileShader(m_ShaderID);

	// Check compilation status
	GLint success;
	glGetShaderiv(m_ShaderID, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		GLchar infoLog[1024];
		glGetShaderInfoLog(m_ShaderID, sizeof(infoLog), nullptr, infoLog);
		fprintf(stderr, "Compute shader compilation failed: %s\n", infoLog);
		return false;
	}

	return true;
}

bool ComputeShader::LinkProgram()
{
	m_ProgramID = glCreateProgram();
	glAttachShader(m_ProgramID, m_ShaderID);
	glLinkProgram(m_ProgramID);

	// Check linking status
	GLint success;
	glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &success);

	if (!success)
	{
		GLchar infoLog[1024];
		glGetProgramInfoLog(m_ProgramID, sizeof(infoLog), nullptr, infoLog);
		fprintf(stderr, "Compute shader program linking failed: %s\n", infoLog);
		return false;
	}

	// Detach shader after successful linking
	glDetachShader(m_ProgramID, m_ShaderID);

	return true;
}

bool ComputeShader::LoadFromFile(const std::string &filePath)
{
	// Read file contents (implementation depends on your file handling approach)
	std::string fileContents = ""; // Replace with actual file reading code

	m_Name = filePath; // Set the name to the file path

	// Use a file reading method of your choice
	// For example, using std::ifstream:

	std::ifstream file(filePath);
	if (!file.is_open())
	{
		fprintf(stderr, "Failed to open file: %s\n", filePath.c_str());
		return false;
	}

	std::stringstream buffer;
	buffer << file.rdbuf();
	fileContents = buffer.str();
	return LoadFromString(fileContents);
}

bool ComputeShader::LoadFromString(const std::string &shaderSource)
{
	if (!CompileShader(shaderSource))
	{
		return false;
	}

	if (!LinkProgram())
	{
		return false;
	}

	return true;
}

void ComputeShader::Use()
{
	glUseProgram(m_ProgramID);
}

void ComputeShader::Dispatch(GLuint numGroupsX, GLuint numGroupsY, GLuint numGroupsZ)
{
	glDispatchCompute(numGroupsX, numGroupsY, numGroupsZ);
	Wait(); // Wait for completion after dispatching
}

void ComputeShader::Wait()
{
	// Memory barrier to ensure compute shader writes are visible
	glMemoryBarrier(GL_ALL_BARRIER_BITS);
	// Alternatively, you can use specific barriers as needed:
	// GL_SHADER_STORAGE_BARRIER_BIT, GL_SHADER_IMAGE_ACCESS_BARRIER_BIT, etc.
}
