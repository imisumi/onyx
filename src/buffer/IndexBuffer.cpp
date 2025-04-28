#include "IndexBuffer.h"

#include <GL/glew.h>

IndexBuffer::IndexBuffer(uint32_t *indices, uint32_t count)
	: m_Count(count)
{
	glCreateBuffers(1, &m_ProgramId);
	// glNamedBufferData(m_ProgramId, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ProgramId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
}

IndexBuffer::IndexBuffer(const std::vector<uint32_t> &indices)
	: m_Count(static_cast<uint32_t>(indices.size()))
{
	glCreateBuffers(1, &m_ProgramId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ProgramId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_ProgramId);
}

void IndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ProgramId);
}

void IndexBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
