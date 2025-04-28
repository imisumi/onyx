#include "Application.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer/Renderer.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

Application::Application()
{
	m_Window = Window::Create("Onyx", 2560, 1440, false);
	// m_Scene = std::make_shared<Scene>();
	// m_Scene->SetCamera(std::make_shared<Camera>());
	// m_Scene->SetMesh(Mesh::CreateDefaultCube());

	//? Move to init UI?
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	// ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(m_Window->GetNativeWindow(), true);
	ImGui_ImplOpenGL3_Init(m_Window->GetGLSLVersion());

	//? Temp

	glGenVertexArrays(1, &m_GridVAO);
	glBindVertexArray(m_GridVAO);
	glBindVertexArray(0);

	m_Camera = std::make_shared<Camera>(m_Window->GetWidth(), m_Window->GetHeight(), glm::vec3(0.0f, 2.0f, 2.0f));
	m_Scene = std::make_shared<Scene>();
	m_Scene->SetCamera(m_Camera);

	// m_GridShader.initialize();

	m_GridShader = std::make_shared<GridShader>(
		200,                                // Grid size
		1.0f,                               // Cell size
		glm::vec4(0.3f, 0.3f, 0.3f, 0.7f),  // Regular grid
		glm::vec4(0.5f, 0.5f, 0.5f, 0.7f),  // Main grid
		glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),  // X-axis
		glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),  // Z-axis
		glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),  // Y-axis
		0.0f,                             // Larger fade distance
		100.0f                              // Even larger max distance
	);
}

Application::~Application()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// m_Window->Shutdown();
}

void Application::Run()
{
	float lastFrameTime = glfwGetTime();
	while (!m_Window->ShouldClose())
	{
		float currentTime = glfwGetTime();
		float deltaTime = currentTime - lastFrameTime;
		lastFrameTime = currentTime;

		Timestep timestep(deltaTime);

		m_Window->PollEvents();

		RenderCommand::SetClearColor(m_ClearColor);
		RenderCommand::Clear();

		//? temporary delta time
		OnUpdate(timestep);

		OnRender();

		OnUIRender();

		m_Window->SwapBuffers();
	}
}

void Application::OnUpdate(Timestep deltaTime)
{
	m_Camera->SetWidth(m_Window->GetWidth());
	m_Camera->SetHeight(m_Window->GetHeight());
	if (!ImGui::GetIO().WantCaptureMouse && !ImGui::GetIO().WantCaptureKeyboard)
	{
		m_Camera->Inputs(m_Window->GetNativeWindow(), deltaTime);
	}
}

void Application::OnRender()
{
	m_Scene->Render();

	glm::vec3 cameraPos = m_Camera->GetPosition();
	cameraPos.y = 0.0f; // Set the camera position to the ground level
	m_GridShader->render(m_Camera->GetViewMatrix(), m_Camera->GetProjectionMatrix(45.0f, 0.1f, 100.0f),cameraPos);
}

void Application::OnUIRender()
{
	ImGuiIO &io = ImGui::GetIO();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	ImGui::ColorEdit3("clear color", (float *)&m_ClearColor); // Edit 3 floats representing a color

	ImGui::End(); // End the window

	m_Scene->OnImGuiRender();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
