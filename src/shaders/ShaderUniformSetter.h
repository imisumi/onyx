#pragma once

#include <string>
#include <string_view>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

#include "core/Base.h"

template <typename DerivedShader>
class ShaderUniformSetter
{
public:
	void SetBool(const std::string &name, bool value)
	{
		int location = GetUniformLocation(name);
		if (location != -1)
			glUniform1i(location, value ? 1 : 0);
	}

	void SetInt(const std::string &name, int value)
	{
		int location = GetUniformLocation(name);
		if (location != -1)
			glUniform1i(location, value);
	}

	void SetUnsignedInt(const std::string &name, uint32_t value)
	{
		int location = GetUniformLocation(name);
		if (location != -1)
			glUniform1ui(location, value);
	}

	void SetFloat(const std::string &name, float value)
	{
		int location = GetUniformLocation(name);
		if (location != -1)
			glUniform1f(location, value);
	}
	void SetFloat2(const std::string &name, const glm::vec2 &value)
	{
		int location = GetUniformLocation(name);
		if (location != -1)
			glUniform2f(location, value.x, value.y);
	}
	void SetFloat3(const std::string &name, const glm::vec3 &value)
	{
		int location = GetUniformLocation(name);
		if (location != -1)
			glUniform3f(location, value.x, value.y, value.z);
	}
	void SetFloat4(const std::string &name, const glm::vec4 &value)
	{
		int location = GetUniformLocation(name);
		if (location != -1)
			glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void SetMat3(const std::string &name, const glm::mat3 &value)
	{
		int location = GetUniformLocation(name);
		if (location != -1)
			glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}
	void SetMat4(const std::string &name, const glm::mat4 &value)
	{
		int location = GetUniformLocation(name);
		if (location != -1)
			glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

private:
	int GetUniformLocation(const std::string &name)
	{
		if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
			return m_UniformLocationCache[name];

		uint32_t programId = static_cast<DerivedShader *>(this)->GetProgramID();
		GLint location = glGetUniformLocation(programId, name.c_str());
		if (location == -1)
		{
			// LOG_ERROR("Warning: uniform '{0}' doesn't exist!", name);
			LOG_ERROR("Warning: uniform '{0}' doesn't exist in shader '{1}'!", name, static_cast<DerivedShader *>(this)->GetName());
			return -1;
		}

		m_UniformLocationCache[name] = static_cast<int>(location);
		return static_cast<int>(location);
	}

private:
	std::unordered_map<std::string, int> m_UniformLocationCache;
};