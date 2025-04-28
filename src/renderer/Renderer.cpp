#include "Renderer.h"

#include <GL/glew.h>

void Renderer::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Renderer::SetClearColor(const glm::vec4 &color)
{
	RenderCommand::SetClearColor(color);
}

void Renderer::Submit(const std::shared_ptr<VertexArray> &vertexArray)
{
	vertexArray->Bind();
	RenderCommand::DrawIndexed(vertexArray);
	// vertexArray->Unbind();
}

void Renderer::SubmitInstanced(const std::shared_ptr<VertexArray> &vertexArray, uint32_t count)
{
	vertexArray->Bind();
	glDrawElementsInstanced(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr, count);
	// vertexArray->Unbind();
}

void Renderer::SubmitInstancedWireframe(const std::shared_ptr<VertexArray> &vertexArray, uint32_t count)
{
	vertexArray->Bind();
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// glDrawElementsInstanced(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr, count);
	// glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	// vertexArray->Unbind();
	glDrawElementsInstanced(GL_LINES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr, count);

}

void Renderer::BeginScene()
{
	// Set up the scene (e.g., camera, lighting, etc.)
}

void Renderer::EndScene()
{
	// Clean up the scene (e.g., reset states, etc.)
}

//? RenderCommand

void RenderCommand::DrawIndexed(const std::shared_ptr<VertexArray> &vertexArray)
{
	glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
}

void RenderCommand::SetClearColor(const glm::vec4 &color)
{
	glClearColor(color.r, color.g, color.b, color.a);
}

void RenderCommand::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}