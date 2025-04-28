#pragma once

#include <memory>
#include <vector>
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void Bind() const;
	void Unbind() const;

	void AddVertexBuffer(const std::shared_ptr<VertexBuffer> &vertexBuffer);
	void SetIndexBuffer(const std::shared_ptr<IndexBuffer> &indexBuffer);

	const std::vector<std::shared_ptr<VertexBuffer>> &GetVertexBuffers() const { return m_VertexBuffers; }
	const std::shared_ptr<IndexBuffer> &GetIndexBuffer() const { return m_IndexBuffer; }
	const uint32_t GetId() const { return m_ProgramId; }

private:
	uint32_t m_ProgramId;
	std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
	std::shared_ptr<IndexBuffer> m_IndexBuffer;
	bool m_Initialized = false;
};