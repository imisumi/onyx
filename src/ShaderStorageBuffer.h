// #pragma once

// #include <iostream>

// class ShaderStorageBuffer
// {

// public:
// 	ShaderStorageBuffer();
// 	~ShaderStorageBuffer();
// 	// Initialize with data
// 	void init(const void *data, size_t size, uint32_t bindingPoint, uint32_t usage);
// 	// Initialize empty buffer
// 	void initEmpty(size_t size, uint32_t bindingPoint, uint32_t usage);
// 	// Resize buffer (useful for your per-frame reconstruction)
// 	void resize(size_t newSize, uint32_t usage);

// 	void clear(uint32_t usage);

// 	uint32_t getId() const { return m_id; }

// 	void bind() const;

// 	void unbind() const;

// 	// Update buffer data
// 	void setData(const void *data, size_t size, size_t offset = 0);

// private:
// 	uint32_t m_id = 0;
// 	size_t m_size = 0;
// 	uint32_t m_bindingPoint = 0;
// 	bool m_initialized = false;
// };

#pragma once

#include <iostream>

class ShaderStorageBuffer
{

public:
	ShaderStorageBuffer();
	~ShaderStorageBuffer();
	// Initialize with data
	void init(const void *data, size_t size, uint32_t bindingPoint, uint32_t usage);
	// Initialize empty buffer
	void initEmpty(size_t size, uint32_t bindingPoint, uint32_t usage);
	// Resize buffer (useful for your per-frame reconstruction)
	void resize(size_t newSize, uint32_t usage);

	void clear(uint32_t usage);
	
	// New methods
	// Push data to the buffer, resizing if necessary
	void pushData(const void *data, size_t dataSize, uint32_t usage);
	// Reset the occupied size counter to 0 without modifying data
	void resetSize();
	// Reset size counter and zero out all data
	void fullClear(uint32_t usage);

	uint32_t getId() const { return m_id; }

	void bind() const;

	void unbind() const;

	// Update buffer data
	void setData(const void *data, size_t size, size_t offset = 0);
	
	// Get current occupied size and total capacity
	size_t getSize() const { return m_occupied_size; }
	size_t getCapacity() const { return m_size; }

private:
	uint32_t m_id = 0;
	size_t m_size = 0;         // Total capacity of the buffer
	size_t m_occupied_size = 0; // Currently used size
	uint32_t m_bindingPoint = 0;
	bool m_initialized = false;
};