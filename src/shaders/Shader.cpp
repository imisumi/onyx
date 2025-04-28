#include "Shader.h"

#include <GL/glew.h>
#include <iostream>

#include <glm/gtc/type_ptr.hpp>

#include <assert.h>
#include <filesystem>
#include <fstream>
#include <sstream>

#include "core/Base.h"

void Shader::CreateProgram(GLuint vertexShader, GLuint fragmentShader)
{
	m_RendererID = glCreateProgram();
	glAttachShader(m_RendererID, vertexShader);
	glAttachShader(m_RendererID, fragmentShader);
	glLinkProgram(m_RendererID);

	GLint isLinked = 0;
	glGetProgramiv(m_RendererID, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, &infoLog[0]);

		// We don't need the program anymore.
		glDeleteProgram(m_RendererID);
		// Don't leak shaders either.
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		MY_ASSERT("{0}", infoLog.data());
		MY_ASSERT(false, "Shader link failure!");
		return;
	}

	glDetachShader(m_RendererID, vertexShader);
	glDetachShader(m_RendererID, fragmentShader);
}

void Shader::CreateProgram(GLuint vertexShader, GLuint fragmentShader, GLuint geometryShader)
{
	m_RendererID = glCreateProgram();
	glAttachShader(m_RendererID, vertexShader);
	glAttachShader(m_RendererID, fragmentShader);
	glAttachShader(m_RendererID, geometryShader);
	glLinkProgram(m_RendererID);

	GLint isLinked = 0;
	glGetProgramiv(m_RendererID, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, &infoLog[0]);

		// We don't need the program anymore.
		glDeleteProgram(m_RendererID);
		// Don't leak shaders either.
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		glDeleteShader(geometryShader);

		LOG_ERROR("{0}", infoLog.data());
		MY_ASSERT(false, "Shader link failure: {0}", infoLog.data());
		return;
	}

	glDetachShader(m_RendererID, vertexShader);
	glDetachShader(m_RendererID, fragmentShader);
	glDetachShader(m_RendererID, geometryShader);
}

GLuint Shader::CompileShader(GLenum shaderType, const std::string &shaderCode)
{
	std::string shaderTypeDefine;
	std::string shaderSource;

	if (shaderType == GL_VERTEX_SHADER)
		shaderTypeDefine = "#define _TYPE_VERT_SHADER\n";
	else if (shaderType == GL_FRAGMENT_SHADER)
		shaderTypeDefine = "#define _TYPE_FRAG_SHADER\n";
	else if (shaderType == GL_GEOMETRY_SHADER)
		shaderTypeDefine = "#define _TYPE_GEOM_SHADER\n";
	else if (shaderType == GL_COMPUTE_SHADER)
		shaderTypeDefine = "#define _TYPE_COMP_SHADER\n";
	else
	{
		MY_ASSERT(false, "Unknown shader type");
		return 0;
	}

	// auto [versionDirective, remainingCode] = extractVersionDirective(shaderCode);
	// std::string finalShaderCode = versionDirective + shaderTypeDefine + remainingCode;
	// const char *codeArray = finalShaderCode.c_str();
	// const char* codeArray = shaderSource.c_str();

	// std::cout << codeArray << std::endl;
	// std::string finalCode = codeArray;
	// MY_ASSERT("\n{0}", finalCode);
	// exit(0);

	GLuint shader = glCreateShader(shaderType);
	if (shader == 0)
	{
		MY_ASSERT(false, "Failed to create shader!");
		return 0;
	}

	const char *src = shaderCode.c_str();
	// glShaderSource(shader, 1, &codeArray, NULL);
	glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);

	// check for shader compile errors
	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLint length = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		std::vector<GLchar> log(length);
		glGetShaderInfoLog(shader, length, &length, &log[0]);
		std::string type = shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment";
		std::cout << "Failed to compile " << type << " shader: " << log.data() << std::endl;

		MY_ASSERT(false, "Failed to compile {0} shader: {1}", type, log.data());
		// MY_ASSERT(false, "Shader compilation failure!");
		glDeleteShader(shader);
		return 0;
	}

	return shader;
}

Shader::~Shader()
{
	glDeleteProgram(m_RendererID);
}

void Shader::Bind() const
{
	MY_ASSERT(m_RendererID, "Shader not created!");
	glUseProgram(m_RendererID);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}

std::string Shader::ReadFile(const std::string &filepath)
{
	std::string result;
	std::ifstream in(filepath, std::ios::in | std::ios::binary);
	if (in)
	{
		in.seekg(0, std::ios::end);
		result.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&result[0], result.size());
		in.close();
	}
	else
	{
		MY_ASSERT(false, "Could not open file '{0}'", filepath);
	}

	return result;
}

std::pair<std::string, std::string> Shader::extractVersionDirective(const std::string &shaderCode)
{
	std::istringstream stream(shaderCode);
	std::string line;
	std::string versionDirective;
	std::string remainingCode;
	bool versionFound = false;

	while (std::getline(stream, line))
	{
		if (!versionFound && line.find("#version") != std::string::npos)
		{
			versionDirective = line + "\n";
			versionFound = true;
		}
		else
		{
			remainingCode += line + "\n";
		}
	}

	return {versionDirective, remainingCode};
}

std::shared_ptr<Shader> Shader::Create(const std::string &filepath)
{
	return std::make_shared<Shader>(filepath);
}

std::shared_ptr<Shader> Shader::Create(const std::string &name, const std::string &vertexSrc, const std::string &fragmentSrc)
{
	return std::make_shared<Shader>(name, vertexSrc, fragmentSrc);
}

std::shared_ptr<Shader> Shader::Create(const std::string &name, const std::string &vertexSrc, const std::string &fragmentSrc, const std::string &geometrySrc)
{
	return std::make_shared<Shader>(name, vertexSrc, fragmentSrc, geometrySrc);
}

void ShaderLibrary::Add(const std::shared_ptr<Shader> &shader)
{
	const std::string &name = shader->GetName();
	Add(name, shader);
}

void ShaderLibrary::Add(const std::string &name, const std::shared_ptr<Shader> &shader)
{
	MY_ASSERT(!Exists(name), "Shader already exists!");
	m_Shaders[name] = shader;
}

std::shared_ptr<Shader> ShaderLibrary::Load(const std::string &filepath)
{
	auto shader = Shader::Create(filepath);
	Add(shader);

	return shader;
}
std::shared_ptr<Shader> ShaderLibrary::Load(const std::string &name, const std::string &filepath)
{
	auto shader = Shader::Create(filepath);
	Add(name, shader);
	return shader;
}

std::shared_ptr<Shader> ShaderLibrary::Get(const std::string &name)
{
	MY_ASSERT(Exists(name), "Shader not found!");
	return m_Shaders[name];
}

bool ShaderLibrary::Exists(const std::string &name) const
{
	return m_Shaders.find(name) != m_Shaders.end();
}

Shader::Shader(const std::string &filepath)
{
	std::string source = ReadFile(filepath);

	GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, source);
	GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, source);

	CreateProgram(vertexShader, fragmentShader);

	std::filesystem::path path = filepath;
	m_Name = path.stem().string(); // Returns the file's name stripped of the extension.
}

Shader::Shader(const std::string &name, const std::string &vertexSrc, const std::string &fragmentSrc)
	: m_Name(name)
{
	GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, ReadFile(vertexSrc));
	GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, ReadFile(fragmentSrc));

	CreateProgram(vertexShader, fragmentShader);
}

Shader::Shader(const std::string &name, const std::string &vertexSrc, const std::string &fragmentSrc, const std::string &geometrySrc)
	: m_Name(name)
{
	GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, ReadFile(vertexSrc));
	GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, ReadFile(fragmentSrc));
	GLuint geometryShader = CompileShader(GL_GEOMETRY_SHADER, ReadFile(geometrySrc));

	CreateProgram(vertexShader, fragmentShader, geometryShader);
}