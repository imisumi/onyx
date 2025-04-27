#pragma once

#include <glm/glm.hpp>

#include <memory>
#include "VertexArray.h"

class Renderer
{
public:
	void Clear();
	void SetClearColor(const glm::vec4 &color);

	void DrawIndexed(const std::shared_ptr<VertexArray> &vertexArray);




	static void BeginScene();
	static void EndScene();

	static void Submit(const std::shared_ptr<VertexArray> &vertexArray);
	static void SubmitInstanced(const std::shared_ptr<VertexArray> &vertexArray, uint32_t count);
	static void SubmitInstancedWireframe(const std::shared_ptr<VertexArray> &vertexArray, uint32_t count);


private:
	// glm::vec4 m_ClearColor = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
};

class RenderCommand
{
public:
	static void DrawIndexed(const std::shared_ptr<VertexArray> &vertexArray);

	static void SetClearColor(const glm::vec4 &color);
	static void Clear();
	static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
private:
};