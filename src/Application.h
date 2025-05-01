#pragma once

#include "platform/Window.h"

#include "core/Timestep.h"

#include <glm/glm.hpp>

#include <scene/Scene.h>

#include "GridShader.h"

#include "buffer/FrameBuffer.h"
#include "renderer/Texture.h"
#include "Shaders/ComputeShader.h"
#include "renderer/EditorCamera.h"


class Application
{
public:
	Application();
	~Application();

	void Run();

	static Application &Get() { return *s_Instance; }

	static Window &GetWindow() { return *s_Instance->m_Window; }

private:
	void OnUpdate(Timestep deltaTime);
	void OnRender();
	void OnUIRender();

private:
	static Application *s_Instance;
	std::unique_ptr<Window> m_Window;
	bool m_Running = true;

	glm::vec4 m_ClearColor = glm::vec4(0.0f, 0.078f, 0.118f, 1.0f);

	uint32_t m_GridVAO = 0;

	std::shared_ptr<Scene> m_Scene;
	std::shared_ptr<EditorCamera> m_EditorCamera;

	// GridShader m_GridShader;
	// GridShader m_GridShader = GridShader(20, 0.5f, glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));
	std::shared_ptr<GridShader> m_GridShader;

	std::shared_ptr<FrameBuffer> m_ViewportFB;
	std::shared_ptr<FrameBuffer> m_RenderViewFB;
	std::shared_ptr<Texture2D> m_RenderViewTexture;
	std::shared_ptr<ComputeShader> m_ComputeShader;
	std::shared_ptr<ComputeShader> m_ComputeRGBA32FToRGBA8;

	// EditorCamera m_EditorCamera;

	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
	bool m_ViewportResized = false;
	bool m_ViewportFocused = false;
	bool m_ViewportHovered = false;

	bool m_CameraControl = false;
};