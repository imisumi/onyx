// #include "ShaderStorageBuffer.h"

// #include <GL/glew.h>

// ShaderStorageBuffer::ShaderStorageBuffer()
// 	: m_id(0), m_size(0), m_bindingPoint(0), m_initialized(false)
// {
// }

// ShaderStorageBuffer::~ShaderStorageBuffer()
// {
// 	if (m_initialized)
// 	{
// 		glDeleteBuffers(1, &m_id);
// 	}
// }

// void ShaderStorageBuffer::init(const void *data, size_t size, uint32_t bindingPoint, GLenum usage)
// {
// 	m_size = size;
// 	m_bindingPoint = bindingPoint;

// 	if (m_initialized)
// 	{
// 		glDeleteBuffers(1, &m_id);
// 	}

// 	glGenBuffers(1, &m_id);
// 	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_id);
// 	glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, usage);
// 	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingPoint, m_id);

// 	m_initialized = true;
// }

// void ShaderStorageBuffer::initEmpty(size_t size, uint32_t bindingPoint, GLenum usage)
// {
// 	init(nullptr, size, bindingPoint, usage);
// }

// void ShaderStorageBuffer::resize(size_t newSize, GLenum usage)
// {
// 	if (newSize != m_size)
// 	{
// 		m_size = newSize;
// 		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_id);
// 		glBufferData(GL_SHADER_STORAGE_BUFFER, newSize, nullptr, usage);
// 		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_bindingPoint, m_id);
// 	}
// }

// void ShaderStorageBuffer::clear(GLenum usage)
// {
//     if (m_initialized)
//     {
//         glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_id);
//         glBufferData(GL_SHADER_STORAGE_BUFFER, m_size, nullptr, usage);
//         glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
//     }
// }

// void ShaderStorageBuffer::bind() const
// {
// 	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_id);
// }

// void ShaderStorageBuffer::unbind() const
// {
// 	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
// }

// void ShaderStorageBuffer::setData(const void *data, size_t size, size_t offset)
// {
// 	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_id);
// 	glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, size, data);
// 	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
// }

#include "ShaderStorageBuffer.h"

#include <GL/glew.h>
#include <cstring> // For memcpy

ShaderStorageBuffer::ShaderStorageBuffer()
	: m_id(0), m_size(0), m_occupied_size(0), m_bindingPoint(0), m_initialized(false)
{
}

ShaderStorageBuffer::~ShaderStorageBuffer()
{
	if (m_initialized)
	{
		glDeleteBuffers(1, &m_id);
	}
}

void ShaderStorageBuffer::init(const void *data, size_t size, uint32_t bindingPoint, GLenum usage)
{
	m_size = size;
	m_occupied_size = data ? size : 0; // If data is provided, occupied size equals total size
	m_bindingPoint = bindingPoint;

	if (m_initialized)
	{
		glDeleteBuffers(1, &m_id);
	}

	glGenBuffers(1, &m_id);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_id);
	glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, usage);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingPoint, m_id);

	m_initialized = true;
}

void ShaderStorageBuffer::initEmpty(size_t size, uint32_t bindingPoint, GLenum usage)
{
	init(nullptr, size, bindingPoint, usage);
	m_occupied_size = 0; // Explicitly set occupied size to 0 for empty buffers
}

void ShaderStorageBuffer::resize(size_t newSize, GLenum usage)
{
	if (newSize != m_size)
	{
		// Save current data if we have any
		void* tempData = nullptr;
		if (m_occupied_size > 0)
		{
			tempData = malloc(m_occupied_size);
			if (tempData)
			{
				glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_id);
				glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, m_occupied_size, tempData);
			}
		}
		
		// Resize the buffer
		m_size = newSize;
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_id);
		glBufferData(GL_SHADER_STORAGE_BUFFER, newSize, nullptr, usage);
		
		// Restore data if we had any
		if (tempData)
		{
			// Only copy up to the new size if it's smaller
			size_t copySize = (m_occupied_size <= newSize) ? m_occupied_size : newSize;
			glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, copySize, tempData);
			
			// Update occupied size if we had to truncate
			if (m_occupied_size > newSize)
				m_occupied_size = newSize;
				
			free(tempData);
		}
		
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_bindingPoint, m_id);
	}
}

void ShaderStorageBuffer::clear(GLenum usage)
{
    if (m_initialized)
    {
        // Bind the buffer
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_id);
        
        // Reallocate with nullptr (zeros out all data)
        glBufferData(GL_SHADER_STORAGE_BUFFER, m_size, nullptr, usage);
        
        // Ensure we rebind to the binding point
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_bindingPoint, m_id);
        
        // Unbind
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
        
        // Reset occupied size counter
        m_occupied_size = 0;
    }
}

void ShaderStorageBuffer::pushData(const void *data, size_t dataSize, GLenum usage)
{
    if (!m_initialized)
        return;
        
    // Check if we need to resize the buffer
    if (m_occupied_size + dataSize > m_size)
    {
        // Calculate new size (double the capacity, or at least fit the new data)
        size_t newSize = m_size * 2;
        if (newSize < m_occupied_size + dataSize)
            newSize = m_occupied_size + dataSize;
            
        // Resize the buffer
        resize(newSize, usage);
    }
    
    // Push the data at the current position
    if (data != nullptr && dataSize > 0)
    {
        // Make sure the buffer is bound before setting data
        bind();
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, m_occupied_size, dataSize, data);
        unbind();
    }
    
    // Update occupied size
    m_occupied_size += dataSize;
}

void ShaderStorageBuffer::resetSize()
{
    m_occupied_size = 0;
    // Note: This doesn't modify the actual buffer data, just resets the counter
}

void ShaderStorageBuffer::fullClear(GLenum usage)
{
    if (m_initialized)
    {
        // Zero out all the data in the buffer
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_id);
        
        // Option 1: Use glBufferData to replace the entire buffer with nulls
        glBufferData(GL_SHADER_STORAGE_BUFFER, m_size, nullptr, usage);
        
        // Option 2: Use glClearBufferData to set all data to zero (if available)
        // GLuint pattern = 0;
        // glClearBufferData(GL_SHADER_STORAGE_BUFFER, GL_R8UI, GL_RED_INTEGER, GL_UNSIGNED_BYTE, &pattern);
        
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
        
        // Reset the occupied size counter
        m_occupied_size = 0;
    }
}

void ShaderStorageBuffer::bind() const
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_id);
}

void ShaderStorageBuffer::unbind() const
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void ShaderStorageBuffer::setData(const void *data, size_t size, size_t offset)
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_id);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, size, data);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}