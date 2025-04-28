// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <stdio.h>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GL/glew.h>	// Include GLEW before GLFW
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "renderer/Camera.h"

#include "shaders/Shader.h"

#include "core/Base.h"

#include <iostream>
#include "buffer/ShaderStorageBuffer.h"
#include "shaders/ComputeShader.h"
#include "buffer/VertexBuffer.h"
#include "buffer/IndexBuffer.h"
#include "buffer/VertexArray.h"

#include "renderer/Renderer.h"

#include "scene/Mesh.h"

#include "renderer/Material.h"

#include "scene/Scene.h"

static void glfw_error_callback(int error, const char *description)
{
	fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

#include <string>

void APIENTRY debugCallback(GLenum source, GLenum type, GLuint id,
							GLenum severity, GLsizei length,
							const GLchar *message, const void *userParam)
{
	(void)length;	 // Unused parameter
	(void)userParam; // Unused parameter

	// Ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
		return;

	std::string debugMsg;
	debugMsg += "Debug message (" + std::to_string(id) + "): " + message + "\n";

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:
		debugMsg += "Source: API\n";
		break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		debugMsg += "Source: Window System\n";
		break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		debugMsg += "Source: Shader Compiler\n";
		break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		debugMsg += "Source: Third Party\n";
		break;
	case GL_DEBUG_SOURCE_APPLICATION:
		debugMsg += "Source: Application\n";
		break;
	case GL_DEBUG_SOURCE_OTHER:
		debugMsg += "Source: Other\n";
		break;
	}

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:
		debugMsg += "Type: Error\n";
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		debugMsg += "Type: Deprecated Behavior\n";
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		debugMsg += "Type: Undefined Behavior\n";
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		debugMsg += "Type: Portability\n";
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		debugMsg += "Type: Performance\n";
		break;
	case GL_DEBUG_TYPE_MARKER:
		debugMsg += "Type: Marker\n";
		break;
	case GL_DEBUG_TYPE_PUSH_GROUP:
		debugMsg += "Type: Push Group\n";
		break;
	case GL_DEBUG_TYPE_POP_GROUP:
		debugMsg += "Type: Pop Group\n";
		break;
	case GL_DEBUG_TYPE_OTHER:
		debugMsg += "Type: Other\n";
		break;
	}

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:
		debugMsg += "Severity: high\n";
		LOG_CRITICAL(debugMsg.c_str());
		exit(-1);
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		debugMsg += "Severity: medium\n";
		LOG_ERROR(debugMsg.c_str());
		break;
	case GL_DEBUG_SEVERITY_LOW:
		debugMsg += "Severity: low\n";
		LOG_WARN(debugMsg.c_str());
		break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		debugMsg += "Severity: notification\n";
		LOG_INFO(debugMsg.c_str());
		break;
	}
}

// In your initialization code:
void setupDebugOutput()
{
	// Make sure you've created a debug context

	// During GLFW initialization (if using GLFW):
	// glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	// Enable debug output
	glEnable(GL_DEBUG_OUTPUT);
	// Make the callback be called synchronously (while the error happens)
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	// Register the callback
	glDebugMessageCallback(debugCallback, nullptr);

	// Control which messages are reported
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

	// You can also filter specific messages:
	// glDebugMessageControl(GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_ERROR, GL_DONT_CARE, 0, nullptr, GL_TRUE);
}

// Main code
int main(int, char **)
{
	Log::Init();
	LOG_TRACE("Trace message");
	LOG_DEBUG("Debug message");
	LOG_INFO("Info message");
	LOG_WARN("Warn message");
	LOG_ERROR("Error message");
	LOG_CRITICAL("Critical message");

	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		return 1;

	// GL 3.0 + GLSL 130
	const char *glsl_version = "#version 460";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	// setupDebugCallback();

	int width = 2560;
	int height = 1440;
	// Create window with graphics context
	GLFWwindow *window = glfwCreateWindow(width, height, "Dear ImGui GLFW+OpenGL3 example", nullptr, nullptr);
	if (window == nullptr)
		return 1;
	glfwMakeContextCurrent(window);
	glfwSwapInterval(0); // Enable vsync

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	GLint maxSSBOBindings;
	glGetIntegerv(GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS, &maxSSBOBindings);
	// std::cout << "Maximum SSBO bindings: " << maxSSBOBindings << std::endl;
	LOG_INFO("Maximum SSBO bindings: {0}", maxSSBOBindings);

	setupDebugOutput();
	// Setup Dear ImGui context
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
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	std::shared_ptr<Scene> m_Scene = std::make_shared<Scene>();

	// MaterialLibrary materialLibrary;
	std::shared_ptr<MaterialLibrary> m_MaterialLibrary = std::make_shared<MaterialLibrary>();

	for (const auto &[name, material] : *m_MaterialLibrary)
	{
		LOG_INFO("Material name: {0}", name);
	}

	std::shared_ptr<ShaderLibrary> shaderLibrary = std::make_shared<ShaderLibrary>();
	shaderLibrary->Add(Shader::Create("GridShader", "Shaders/grid.vert", "Shaders/grid.frag", "Shaders/grid.geom"));
	shaderLibrary->Add(Shader::Create("GridShader2", "Shaders/viewport/grid.vert", "Shaders/viewport/grid.frag"));
	GLuint gridVAO;
	glGenVertexArrays(1, &gridVAO);

	// Grid parameters
	float gridSize = 10.0f;
	float gridSpacing = 1.0f;
	bool showGrid = true;

	// Triangle parameters
	bool showTriangle = true;
	glm::vec3 trianglePosition(0.0f, 0.0f, 0.0f);
	float triangleRotation = 0.0f;
	float triangleScale = 1.0f;

	// Enable depth testing
	glEnable(GL_DEPTH_TEST);

	// Enable blending for transparent grid lines
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// Camera camera(width, height, glm::vec3(0.0f, 2.0f, 2.0f));
	std::shared_ptr<Camera> camera = std::make_shared<Camera>(width, height, glm::vec3(0.0f, 2.0f, 2.0f));
	m_Scene->SetCamera(camera);

	glm::vec3 triangleColor(1.0f, 0.0f, 0.0f); // Red by default

	float previousTime = static_cast<float>(glfwGetTime());
	float deltaTime = 0.0f;

	//? new grid vao
	uint32_t newGridVAO;
	glGenVertexArrays(1, &newGridVAO);
	glBindVertexArray(newGridVAO);
	glBindVertexArray(0);

	ComputeShader computeShader;
	computeShader.LoadFromFile("Shaders/aabb.compute");
	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		// Calculate delta time
		float currentTime = static_cast<float>(glfwGetTime());
		deltaTime = currentTime - previousTime;
		previousTime = currentTime;

		// Poll and handle events (inputs, window resize, etc.)
		glfwPollEvents();
		if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
		{
			ImGui_ImplGlfw_Sleep(10);
			continue;
		}

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text."); // Display some text (you can use a format strings too)
			ImGui::Checkbox("Show Grid", &showGrid);  // Toggle grid visibility

			ImGui::SliderFloat("Grid Size", &gridSize, 5.0f, 20.0f);	  // Control grid size
			ImGui::SliderFloat("Grid Spacing", &gridSpacing, 0.5f, 2.0f); // Control grid spacing

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);			 // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float *)&clear_color); // Edit 3 floats representing a color

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

			// Add to the ImGui controls section
			ImGui::Text("Triangle Controls");
			ImGui::DragFloat3("Triangle Position", glm::value_ptr(trianglePosition), 0.1f);
			ImGui::SliderFloat("Triangle Rotation", &triangleRotation, 0.0f, 360.0f);
			ImGui::SliderFloat("Triangle Scale", &triangleScale, 0.1f, 5.0f);
			ImGui::ColorEdit3("Triangle Color", glm::value_ptr(triangleColor)); // Add color picker

			// ImGui::DragFloat3("Camera Position", glm::type_ptr(camera.GetPosition())); // Control camera position
			glm::vec3 position = camera->GetPosition();						// Copy the position
			ImGui::DragFloat3("Camera Position", glm::value_ptr(position)); // Modify the local copy
			camera->SetPosition(position);									// Update the camera with the modified position
			ImGui::End();
		}

		m_Scene->OnImGuiRender();
		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		// glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		// glClearColor(0.0f, 0.078f, 0.118f, 1.0f);
		// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		RenderCommand::SetClearColor({0.0f, 0.078f, 0.118f, 1.0f});
		RenderCommand::Clear();

		Renderer::BeginScene();

		if (!ImGui::GetIO().WantCaptureMouse && !ImGui::GetIO().WantCaptureKeyboard)
		{
			camera->Inputs(window, deltaTime);
		}

		// Render the grid if enabled
		// if (showGrid)
		// {
		// 	// auto gridShader = shaderLibrary->Get("GridShader");
		// 	std::shared_ptr<Shader> gridShader = shaderLibrary->Get("GridShader");
		// 	gridShader->Bind();

		// 	gridShader->SetMat4("viewProj", camera->Matrix(45.0f, 0.1f, 100.0f));
		// 	gridShader->SetFloat("gridSize", gridSize);
		// 	gridShader->SetFloat("gridSpacing", gridSpacing);

		// 	glBindVertexArray(gridVAO);
		// 	glDrawArrays(GL_POINTS, 0, 1); // Draw a single point, geometry shader creates the grid
		// 	glBindVertexArray(0);
		// }

		auto newGridShader = shaderLibrary->Get("GridShader2");
		newGridShader->Bind();
		newGridShader->SetMat4("viewProjection", camera->Matrix(45.0f, 0.1f, 100.0f));
		newGridShader->SetFloat3("cameraPosition", camera->GetPosition());

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Draw the grid (4 vertices for a quad, using triangle strip)
		glBindVertexArray(newGridVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);

		// Restore state
		glDisable(GL_BLEND);

		m_Scene->Render();

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}

	// Cleanup
	glDeleteVertexArrays(1, &gridVAO);
	// glDeleteProgram(gridProgram);

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
