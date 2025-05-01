#include "Application.h"

// TODO: remove glew from this file
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer/Renderer.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

void SetDarkThemeColors()
{
	auto &colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_WindowBg] = ImVec4{0.1f, 0.105f, 0.11f, 1.0f};

	// Headers
	colors[ImGuiCol_Header] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
	colors[ImGuiCol_HeaderHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
	colors[ImGuiCol_HeaderActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

	// Buttons
	colors[ImGuiCol_Button] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
	colors[ImGuiCol_ButtonHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
	colors[ImGuiCol_ButtonActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

	// Frame BG
	colors[ImGuiCol_FrameBg] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
	colors[ImGuiCol_FrameBgHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
	colors[ImGuiCol_FrameBgActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

	// Tabs
	colors[ImGuiCol_Tab] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
	colors[ImGuiCol_TabHovered] = ImVec4{0.38f, 0.3805f, 0.381f, 1.0f};
	colors[ImGuiCol_TabActive] = ImVec4{0.28f, 0.2805f, 0.281f, 1.0f};
	colors[ImGuiCol_TabUnfocused] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
	// colors[ImGuiCol_TabSelectedOverline] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	// colors[ImGuiCol_TabDimmedSelectedOverline] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

	// Title
	colors[ImGuiCol_TitleBg] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
	colors[ImGuiCol_TitleBgActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
}

Application *Application::s_Instance = nullptr;

Application::Application()
{
	MY_ASSERT(!s_Instance, "Application already exists!");
	s_Instance = this;

	m_Window = Window::Create("Onyx", 2560, 1440, false);

	m_ViewportWidth = m_Window->GetWidth();
	m_ViewportHeight = m_Window->GetHeight();

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
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	SetDarkThemeColors();
	// ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(m_Window->GetNativeWindow(), true);
	ImGui_ImplOpenGL3_Init(m_Window->GetGLSLVersion());

	//? Temp

	glGenVertexArrays(1, &m_GridVAO);
	glBindVertexArray(m_GridVAO);
	glBindVertexArray(0);

	m_Scene = std::make_shared<Scene>();


	// m_GridShader.initialize();

	m_GridShader = std::make_shared<GridShader>(
		200,							   // Grid size
		1.0f,							   // Cell size
		glm::vec4(0.3f, 0.3f, 0.3f, 0.7f), // Regular grid
		glm::vec4(0.5f, 0.5f, 0.5f, 0.7f), // Main grid
		glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), // X-axis
		glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), // Z-axis
		glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), // Y-axis
		0.0f,							   // Larger fade distance
		100.0f							   // Even larger max distance
	);

	// FrameBufferSpecification fbSpec;
	// fbSpec.Width = 2560;
	// fbSpec.Height = 1440;
	// fbSpec.Attachments.Attachments = {
	// 	FrameBufferTextureSpecification{FrameBufferTextureFormat::RGBA8, 1},
	// 	FrameBufferTextureSpecification{FrameBufferTextureFormat::DEPTH24STENCIL8, 1}};

	FrameBufferSpecification frameBufferSpec;
	frameBufferSpec.Attachments = {FrameBufferTextureFormat::RGBA8,
								   //    FrameBufferTextureFormat::RED_INTEGER,
								   FrameBufferTextureFormat::Depth};
	frameBufferSpec.Width = 2560;
	frameBufferSpec.Height = 1440;
	// m_FrameBufferLibrary.Add("EditorFrameBuffer", frameBufferSpec);

	m_ViewportFB = std::make_shared<FrameBuffer>(frameBufferSpec);
	m_RenderViewFB = std::make_shared<FrameBuffer>(frameBufferSpec);
	Texture2DSpecification texSpec;
	texSpec.Width = frameBufferSpec.Width;
	texSpec.Height = frameBufferSpec.Height;
	texSpec.Format = TextureFormat::RGBA32F;
	m_RenderViewTexture = Texture2D::Create(texSpec);

	m_ComputeShader = ComputeShader::Create("Shaders/render/render.comp");
	m_ComputeRGBA32FToRGBA8 = ComputeShader::Create("Shaders/render/convert.comp");

	float aspectRatio = static_cast<float>(m_Window->GetWidth()) / static_cast<float>(m_Window->GetHeight());
	m_EditorCamera = std::make_shared<EditorCamera>(60.0f, aspectRatio, 0.1f, 100.0f);


	m_Scene->SetEditorCamera(m_EditorCamera);
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
	float lastFrameTime = static_cast<float>(glfwGetTime());
	while (!m_Window->ShouldClose())
	{
		float currentTime = static_cast<float>(glfwGetTime());
		float deltaTime = currentTime - lastFrameTime;
		lastFrameTime = currentTime;

		Timestep timestep(deltaTime);

		m_Window->PollEvents();

		RenderCommand::SetClearColor(m_ClearColor);
		RenderCommand::Clear();

		//? temporary delta time
		OnUpdate(timestep);

		// m_ViewportFB->Bind();
		// RenderCommand::SetClearColor(m_ClearColor);
		// RenderCommand::Clear();
		OnRender();
		// m_ViewportFB->Unbind();

		OnUIRender();

		m_Window->SwapBuffers();
	}
}

void Application::OnUpdate(Timestep deltaTime)
{

	//? First we check if any resizing is needed
	if (m_ViewportResized)
	{
		m_ViewportFB->Resize(m_ViewportWidth, m_ViewportHeight);
		m_RenderViewFB->Resize(m_ViewportWidth, m_ViewportHeight);
		m_RenderViewTexture->Resize(m_ViewportWidth, m_ViewportHeight);
		m_ViewportResized = false;
		// RenderCommand::SetViewport(0, 0, m_ViewportWidth, m_ViewportHeight);

		m_EditorCamera->SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	if (m_CameraControl || (m_ViewportFocused && m_ViewportHovered))
	{
		m_CameraControl = m_EditorCamera->OnUpdate(deltaTime);
	}
}

void Application::OnRender()
{
	//? Viewport
	m_ViewportFB->Bind();
	RenderCommand::SetClearColor(m_ClearColor);
	RenderCommand::Clear();
	// RenderCommand::SetViewport(0, 0, m_ViewportWidth, m_ViewportHeight);
	m_Scene->Render();

	glm::vec3 cameraPos = m_EditorCamera->GetPosition(); // Use the editor camera position
	cameraPos.y = 0.0f;						  // Set the camera position to the ground level
	m_GridShader->render(m_EditorCamera->GetViewMatrix(), m_EditorCamera->GetProjectionMatrix(), cameraPos);
	m_ViewportFB->Unbind();

	//? Path tracer
	// m_RenderViewFB->Bind();
	// m_RenderViewTexture->Bind(0);
	// m_ComputeShader->Use();
	// m_ComputeShader->Dispatch(m_ViewportWidth / 8, m_ViewportHeight / 4, 1);
	// m_RenderViewFB->Unbind();
	//? Path tracer
	// Step 1: Run the main compute shader to generate RGBA32F data
	// m_RenderViewTexture->BindImage(0, 0, GL_WRITE_ONLY, GL_RGBA32F);

	uint32_t workGroupsX = (m_ViewportWidth + 7) / 8;  // Ceiling division by 8
	uint32_t workGroupsY = (m_ViewportHeight + 3) / 4; // Ceiling division by 4

	uint32_t renderViewTextureID = m_RenderViewTexture->GetTextureID();
	glBindImageTexture(0, renderViewTextureID, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	m_ComputeShader->Use();

	glm::mat4 viewProjectionMatrix = m_EditorCamera->GetViewProjection();
	glm::mat4 inverseViewProjectionMatrix = glm::inverse(viewProjectionMatrix);

	// m_ComputeShader->SetMat4("viewProjectionMatrix", viewProjectionMatrix);
	m_ComputeShader->SetMat4("viewProjectionInverseMatrix", inverseViewProjectionMatrix);

	m_ComputeShader->Dispatch(workGroupsX, workGroupsY, 1);
	// Assuming your Dispatch already includes memory barrier

	// Step 2: Convert RGBA32F to RGBA8 directly to the framebuffer's color attachment
	m_RenderViewFB->Bind();
	RenderCommand::SetClearColor({0.0f, 0.0f, 0.0f, 1.0f});
	RenderCommand::Clear();
	m_RenderViewFB->Unbind();

	// Bind the RGBA32F texture as input (binding 0)
	// m_RenderViewTexture->BindImage(0, 0, GL_READ_ONLY, GL_RGBA32F);
	// uint32_t renderViewTextureID = m_RenderViewTexture->GetTextureID();
	glBindImageTexture(0, renderViewTextureID, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);

	// Bind the framebuffer's color attachment as output (binding 1)
	uint32_t colorAttachmentID = m_RenderViewFB->GetColorAttachmentRendererID();
	glBindImageTexture(1, colorAttachmentID, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);

	// Run the conversion compute shader
	m_ComputeRGBA32FToRGBA8->Use();
	m_ComputeRGBA32FToRGBA8->Dispatch(workGroupsX, workGroupsY, 1);
	// Assuming your Dispatch already includes memory barrier
}

void Application::OnUIRender()
{
	ImGuiIO &io = ImGui::GetIO();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

	ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	ImGui::ColorEdit3("clear color", (float *)&m_ClearColor); // Edit 3 floats representing a color
	ImGui::Text("Viewport Size: %d x %d", m_ViewportWidth, m_ViewportHeight);
	ImGui::Text("Viewport Focused: %s", m_ViewportFocused ? "true" : "false");
	ImGui::Text("Viewport Hovered : %s", m_ViewportHovered ? "true" : "false");

	glm::vec3 cameraPos = m_EditorCamera->GetPosition();
	ImGui::Text("Camera Position: %f, %f, %f", cameraPos.x, cameraPos.y, cameraPos.z);

	ImGui::End(); // End the window

	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});

		ImGui::Begin("Viewport"); // Create a window called "Hello, world!" and append into it.
		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();

		ImVec2 windowDims = ImGui::GetContentRegionAvail(); // Get the available space in the window
		if (windowDims.x != m_ViewportWidth || windowDims.y != m_ViewportHeight)
		{
			m_ViewportWidth = windowDims.x;
			m_ViewportHeight = windowDims.y;
			m_ViewportResized = true;
		}

		uint64_t textureID = m_ViewportFB->GetColorAttachmentRendererID();
		ImTextureID id = textureID;
		auto spec = m_ViewportFB->GetSpecification();
		// ImGui::Text("Framebuffer size: %d x %d", spec.Width, spec.Height);
		float width = ImGui::GetWindowWidth();
		float height = ImGui::GetWindowHeight();
		ImGui::Image(id, {width, height}, {0, 1}, {1, 0});
		// ImGui::Image(reinterpret_cast<ImTextureID>(reinterpret_cast<uintptr_t>(textureID)), ImVec2{500, 500});
		// ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ 500, 500 }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		// ImGUi::Image(reinterpret_cast<void*>(m_Framebuffer->GetDepthAttachmentRendererID()), ImVec2{ 500, 500 }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End(); // End the window
		ImGui::PopStyleVar();

		//? Render view
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});

		ImGui::Begin("Render view"); // Create a window called "Hello, world!" and append into it.

		uint64_t textureID2 = m_RenderViewFB->GetColorAttachmentRendererID();
		ImTextureID id2 = textureID2;
		// auto spec = m_RenderViewFB->GetSpecification();
		// ImGui::Text("Framebuffer size: %d x %d", spec.Width, spec.Height);
		// float width = ImGui::GetWindowWidth();
		// float height = ImGui::GetWindowHeight();
		ImGui::Image(id2, {width, height}, {0, 1}, {1, 0});
		// ImGui::Image(reinterpret_cast<ImTextureID>(reinterpret_cast<uintptr_t>(textureID)), ImVec2{500, 500});
		// ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ 500, 500 }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		// ImGUi::Image(reinterpret_cast<void*>(m_Framebuffer->GetDepthAttachmentRendererID()), ImVec2{ 500, 500 }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End(); // End the window
		ImGui::PopStyleVar();
	}

	m_Scene->OnImGuiRender();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Update and Render additional Platform Windows
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow *backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}
