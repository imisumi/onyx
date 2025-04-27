#include "VertexArray.h"

#include <GL/glew.h>

static GLint ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
{
	switch (type)
	{
	case ShaderDataType::Float:
		return GL_FLOAT;
	case ShaderDataType::Float2:
		return GL_FLOAT;
	case ShaderDataType::Float3:
		return GL_FLOAT;
	case ShaderDataType::Float4:
		return GL_FLOAT;
	case ShaderDataType::Mat3:
		return GL_FLOAT;
	case ShaderDataType::Mat4:
		return GL_FLOAT;
	case ShaderDataType::Int:
		return GL_INT;
	case ShaderDataType::Int2:
		return GL_INT;
	case ShaderDataType::Int3:
		return GL_INT;
	case ShaderDataType::Int4:
		return GL_INT;
	case ShaderDataType::Bool:
		return GL_BOOL;
	}
	MY_ASSERT(false, "Unknown ShaderDataType!");
	return 0;
}

VertexArray::VertexArray()
{
	glCreateVertexArrays(1, &m_ProgramId);
	// glBindVertexArray(m_ProgramId);
	// glBindVertexArray(0);
	// glBindBuffer(GL_ARRAY_BUFFER, 0);
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

VertexArray::~VertexArray()
{
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_ProgramId);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}

void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer> &vertexBuffer)
{
	MY_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");
	if (m_VertexBuffers.size() > 0)
	{
		MY_ASSERT(m_VertexBuffers[0]->GetLayout().GetElements().size() == vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer layouts do not match!");
	}

	glBindVertexArray(m_ProgramId);
	vertexBuffer->Bind();
	uint32_t index = 0;
	const auto &layout = vertexBuffer->GetLayout();
	for (const auto &element : layout)
	{
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index,
							  element.GetComponentCount(),
							  ShaderDataTypeToOpenGLBaseType(element.Type),
							  element.Normalized ? GL_TRUE : GL_FALSE,
							  layout.GetStride(),
							  (const void *)static_cast<uint64_t>(element.Offset));
		index++;
	}
	m_VertexBuffers.push_back(vertexBuffer);
	//? TODO: Check if this is needed
	glBindVertexArray(0);
}

void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer> &indexBuffer)
{
	glBindVertexArray(m_ProgramId);
	indexBuffer->Bind();
	m_IndexBuffer = indexBuffer;
	glBindVertexArray(0);
}