#pragma once

#include "platform/Window.h"

#include "core/Timestep.h"

#include <glm/glm.hpp>

#include <scene/Scene.h>
#include <renderer/Camera.h>

#include "GridShader.h"

class Application
{
public:
	Application();
	~Application();

	void Run();

private:
	void OnUpdate(Timestep deltaTime);
	void OnRender();
	void OnUIRender();

private:
	std::unique_ptr<Window> m_Window;
	bool m_Running = true;

	glm::vec4 m_ClearColor = glm::vec4(0.0f, 0.078f, 0.118f, 1.0f);


	uint32_t m_GridVAO = 0;

	std::shared_ptr<Scene> m_Scene;
	std::shared_ptr<Camera> m_Camera;

	// GridShader m_GridShader;
	// GridShader m_GridShader = GridShader(20, 0.5f, glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));
	std::shared_ptr<GridShader> m_GridShader;
};