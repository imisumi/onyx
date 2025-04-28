#pragma once

#include <string>
#include <vector>
#include <memory>

class IndexBuffer
{
public:
	IndexBuffer(uint32_t *indices, uint32_t count);
	IndexBuffer(const std::vector<uint32_t> &indices);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	uint32_t GetCount() const { return m_Count; }

	// static IndexBuffer* Create(uint32_t* indices, uint32_t count);
	static std::shared_ptr<IndexBuffer> Create(uint32_t *indices, uint32_t count)
	{
		return std::make_shared<IndexBuffer>(indices, count);
	}

	static std::shared_ptr<IndexBuffer> Create(const std::vector<uint32_t> &indices)
	{
		return std::make_shared<IndexBuffer>(indices);
	}

private:
	uint32_t m_ProgramId;
	uint32_t m_Count;
};
