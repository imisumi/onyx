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
	ImGuiStyle &style = ImGui::GetStyle();
	ImVec4 *colors = style.Colors;

	// Keep the same spacing and rounding
	style.WindowRounding = 6.0f;
	style.WindowBorderSize = 1.0f;
	style.WindowPadding = ImVec2(12, 12);
	style.FramePadding = ImVec2(6, 4);
	style.FrameRounding = 4.0f;
	style.ItemSpacing = ImVec2(8, 6);
	style.ItemInnerSpacing = ImVec2(6, 4);
	style.IndentSpacing = 22.0f;
	style.ScrollbarSize = 14.0f;
	style.ScrollbarRounding = 8.0f;
	style.GrabMinSize = 12.0f;
	style.GrabRounding = 3.0f;
	style.PopupRounding = 4.0f;

	// Base colors
	colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);			// Light grey text (not pure white)
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f); // Medium grey for disabled
	colors[ImGuiCol_WindowBg] = ImVec4(0.12f, 0.12f, 0.12f, 0.95f);		// Dark window background
	colors[ImGuiCol_ChildBg] = ImVec4(0.12f, 0.12f, 0.12f, 0.95f);		// Match window background
	colors[ImGuiCol_PopupBg] = ImVec4(0.14f, 0.14f, 0.14f, 0.95f);		// Slightly darker than window
	colors[ImGuiCol_Border] = ImVec4(0.25f, 0.25f, 0.25f, 0.50f);		// Dark grey border
	colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);		// No shadow

	// Frame colors
	colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.16f, 0.16f, 0.95f);		  // Dark element backgrounds
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.20f, 0.20f, 0.20f, 0.95f); // Slightly lighter on hover
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);  // Even lighter when active

	// Title bar colors
	colors[ImGuiCol_TitleBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);			// Dark grey inactive title
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);	// Slightly lighter active title
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.15f, 0.15f, 0.15f, 0.75f); // Transparent when collapsed
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);		// Slightly darker than title

	// Scrollbar colors
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.14f, 0.14f, 0.14f, 0.95f);			// Scrollbar background
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);		// Scrollbar grab
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f); // Scrollbar grab when hovered
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);	// Scrollbar grab when active

	// Widget colors
	colors[ImGuiCol_CheckMark] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);		// Light grey checkmark
	colors[ImGuiCol_SliderGrab] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);		// Slider grab
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f); // Slider grab when active
	colors[ImGuiCol_Button] = ImVec4(0.20f, 0.20f, 0.20f, 0.80f);			// Dark grey buttons
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);	// Slightly lighter on hover
	colors[ImGuiCol_ButtonActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);		// Even lighter when active

	// Header colors (TreeNode, Selectable, etc)
	colors[ImGuiCol_Header] = ImVec4(0.20f, 0.20f, 0.20f, 0.76f);		 // Pure dark grey
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 0.25f, 0.25f, 0.80f); // Slightly lighter on hover
	colors[ImGuiCol_HeaderActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);	 // Even lighter when active

	// Separator
	colors[ImGuiCol_Separator] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);		// Separator color
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f); // Separator when hovered
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);	// Separator when active

	// Resize grip
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.25f, 0.25f, 0.25f, 0.50f);		 // Resize grip
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.30f, 0.30f, 0.30f, 0.75f); // Resize grip when hovered
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);	 // Resize grip when active

	// Text input cursor
	colors[ImGuiCol_InputTextCursor] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f); // Text input cursor

	// ALL TAB COLORS (both old and new names)
	// Using the newer tab color naming from your enum
	colors[ImGuiCol_Tab] = ImVec4(0.15f, 0.15f, 0.15f, 0.86f);						 // Unselected tab
	colors[ImGuiCol_TabHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.80f);				 // Tab when hovered
	colors[ImGuiCol_TabSelected] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);				 // Selected tab
	colors[ImGuiCol_TabSelectedOverline] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);		 // Selected tab overline
	colors[ImGuiCol_TabDimmed] = ImVec4(0.13f, 0.13f, 0.13f, 0.86f);				 // Dimmed/unfocused tab
	colors[ImGuiCol_TabDimmedSelected] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);		 // Selected but unfocused tab
	colors[ImGuiCol_TabDimmedSelectedOverline] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f); // Overline of unfocused selected tab

	// For backward compatibility with older ImGui versions
	// These might be what your version is using
	if (ImGuiCol_TabActive != ImGuiCol_TabSelected)
	{																			  // Only set if they're different (to avoid warnings)
		colors[ImGuiCol_TabActive] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);		  // Active tab (old name)
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.13f, 0.13f, 0.13f, 0.86f);		  // Unfocused tab (old name)
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f); // Unfocused active tab (old name)
	}

	// Docking colors
	colors[ImGuiCol_DockingPreview] = ImVec4(0.30f, 0.30f, 0.30f, 0.40f); // Preview when docking
	colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f); // Empty docking space

	// Plot colors
	colors[ImGuiCol_PlotLines] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);			// Plot lines
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);		// Plot lines when hovered
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);		// Plot histogram
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f); // Plot histogram when hovered

	// Table colors
	colors[ImGuiCol_TableHeaderBg] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);	 // Table header background
	colors[ImGuiCol_TableBorderStrong] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f); // Table outer borders
	colors[ImGuiCol_TableBorderLight] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);	 // Table inner borders
	colors[ImGuiCol_TableRowBg] = ImVec4(0.14f, 0.14f, 0.14f, 0.90f);		 // Table row background (even)
	colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.16f, 0.16f, 0.16f, 0.90f);	 // Table row background (odd)

	// Miscellaneous
	colors[ImGuiCol_TextLink] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);				 // Light grey for links (not blue)
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.30f, 0.30f, 0.30f, 0.35f);		 // Light grey selection background
	colors[ImGuiCol_TreeLines] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);			 // Tree node lines
	colors[ImGuiCol_DragDropTarget] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);		 // Drag and drop target
	colors[ImGuiCol_NavCursor] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);			 // Navigation cursor
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.40f, 0.40f, 0.40f, 0.70f); // Nav windowing highlight
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.20f);	 // Nav windowing dim
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.15f, 0.15f, 0.15f, 0.75f);		 // Modal window dim
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

		OnRender();

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
	m_Scene->Render();

	glm::vec3 cameraPos = m_EditorCamera->GetPosition(); // Use the editor camera position
	cameraPos.y = 0.0f;									 // Set the camera position to the ground level
	m_GridShader->render(m_EditorCamera->GetViewMatrix(), m_EditorCamera->GetProjectionMatrix(), cameraPos);
	m_ViewportFB->Unbind();

	uint32_t workGroupsX = (m_ViewportWidth + 7) / 8;  // Ceiling division by 8
	uint32_t workGroupsY = (m_ViewportHeight + 3) / 4; // Ceiling division by 4

	uint32_t renderViewTextureID = m_RenderViewTexture->GetTextureID();
	glBindImageTexture(0, renderViewTextureID, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	m_ComputeShader->Use();

	glm::mat4 inverseViewProjectionMatrix = glm::inverse(m_EditorCamera->GetViewProjection());

	// m_ComputeShader->SetMat4("viewProjectionMatrix", viewProjectionMatrix);
	m_ComputeShader->SetMat4("viewProjectionInverseMatrix", inverseViewProjectionMatrix);
	// m_ComputeShader->SetUnsignedInt("numIndices", 224 * 3);
	m_ComputeShader->SetUnsignedInt("numMeshes", m_Scene->GetObjectCount());
	m_ComputeShader->SetFloat3("cameraPosition", m_EditorCamera->GetPosition());

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
	// ImGui::Button("")
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
			m_ViewportWidth = static_cast<uint32_t>(windowDims.x);
			m_ViewportHeight = static_cast<uint32_t>(windowDims.y);
			m_ViewportResized = true;
		}

		uint64_t textureID = m_ViewportFB->GetColorAttachmentRendererID();
		ImTextureID id = textureID;
		auto spec = m_ViewportFB->GetSpecification();
		// ImGui::Text("Framebuffer size: %d x %d", spec.Width, spec.Height);
		float width = ImGui::GetWindowWidth();
		float height = ImGui::GetWindowHeight();
		// ImGui::Image(id, {width, height});
		ImGui::Image(id, {width, height}, {0, 1}, {1, 0});
		ImGui::End(); // End the window
		ImGui::PopStyleVar();

		//? Render view
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});

		ImGui::Begin("Render view"); // Create a window called "Hello, world!" and append into it.

		uint64_t textureID2 = m_RenderViewFB->GetColorAttachmentRendererID();
		ImTextureID id2 = textureID2;
		ImGui::Image(id2, {width, height});
		ImGui::End(); // End the window
		ImGui::PopStyleVar();
	}

	m_Scene->RenderUI();

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
