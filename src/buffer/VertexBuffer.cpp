#include "VertexBuffer.h"

#include <GL/glew.h>

VertexBuffer::VertexBuffer(float *vertices, uint32_t size)
	: m_Size(size)
{
	glCreateBuffers(1, &m_ProgramId);
	glBindBuffer(GL_ARRAY_BUFFER, m_ProgramId);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	// glCreateBuffers(1, &m_ProgramId);
	// glBindBuffer(GL_ARRAY_BUFFER, m_ProgramId);
	// glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

VertexBuffer::VertexBuffer(const std::vector<float> &vertices)
	: m_Size(static_cast<uint32_t>(vertices.size() * sizeof(float)))
{
	glCreateBuffers(1, &m_ProgramId);
	glBindBuffer(GL_ARRAY_BUFFER, m_ProgramId);
	glBufferData(GL_ARRAY_BUFFER, m_Size, vertices.data(), GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_ProgramId);
}

void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_ProgramId);
}

void VertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
