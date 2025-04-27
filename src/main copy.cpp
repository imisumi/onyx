// // Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// // (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)

// // Learn about Dear ImGui:
// // - FAQ                  https://dearimgui.com/faq
// // - Getting Started      https://dearimgui.com/getting-started
// // - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// // - Introduction, links and more at the top of imgui.cpp

// #include "imgui/imgui.h"
// #include "imgui/imgui_impl_glfw.h"
// #include "imgui/imgui_impl_opengl3.h"
// #include <stdio.h>
// #define GL_SILENCE_DEPRECATION
// #if defined(IMGUI_IMPL_OPENGL_ES2)
// #include <GLES2/gl2.h>
// #endif
// #include <GL/glew.h>	// Include GLEW before GLFW
// #include <GLFW/glfw3.h> // Will drag system OpenGL headers
// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>

// #include "Camera.h"

// #include "Shader.h"

// #include "Base.h"

// #include <iostream>
// #include "ShaderStorageBuffer.h"
// #include "ComputeShader.h"
// #include "VertexBuffer.h"
// #include "IndexBuffer.h"
// #include "VertexArray.h"

// static void glfw_error_callback(int error, const char *description)
// {
// 	fprintf(stderr, "GLFW Error %d: %s\n", error, description);
// }

// // static void GLClearErrors()
// // {
// // 	while (glGetError() != GL_NO_ERROR)
// // 		;
// // }
// // static void GLCheckError(const char *function, const char *file, int line)
// // {
// // 	while (GLenum error = glGetError())
// // 	{
// // 		// std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << ":" << line << std::endl;
// // 		LOG_ERROR
// // 	}
// // }

// // void APIENTRY debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
// // 							GLsizei length, const GLchar *message, const void *userParam)
// // {
// // 	// Skip non-significant error/warning codes
// // 	if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
// // 		return;

// // 	std::cerr << "OpenGL: " << message << std::endl;
// // }

// // // Minimal setup
// // void setupDebugCallback()
// // {
// // 	glEnable(GL_DEBUG_OUTPUT);
// // 	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // For easier debugging
// // 	glDebugMessageCallback(debugCallback, nullptr);
// // }

// // The callback function that will receive debug messages
// // void APIENTRY debugCallback(GLenum source, GLenum type, GLuint id,
// // 							GLenum severity, GLsizei length,
// // 							const GLchar *message, const void *userParam)
// // {
// // 	// Ignore non-significant error/warning codes
// // 	if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
// // 		return;

// // 	std::cout << "---------------" << std::endl;
// // 	std::cout << "Debug message (" << id << "): " << message << std::endl;

// // 	LOG_TRACE("---------------");

// // 	switch (source)
// // 	{
// // 	case GL_DEBUG_SOURCE_API:
// // 		std::cout << "Source: API";
// // 		break;
// // 	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
// // 		std::cout << "Source: Window System";
// // 		break;
// // 	case GL_DEBUG_SOURCE_SHADER_COMPILER:
// // 		std::cout << "Source: Shader Compiler";
// // 		break;
// // 	case GL_DEBUG_SOURCE_THIRD_PARTY:
// // 		std::cout << "Source: Third Party";
// // 		break;
// // 	case GL_DEBUG_SOURCE_APPLICATION:
// // 		std::cout << "Source: Application";
// // 		break;
// // 	case GL_DEBUG_SOURCE_OTHER:
// // 		std::cout << "Source: Other";
// // 		break;
// // 	}
// // 	std::cout << std::endl;

// // 	switch (type)
// // 	{
// // 	case GL_DEBUG_TYPE_ERROR:
// // 		std::cout << "Type: Error";
// // 		break;
// // 	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
// // 		std::cout << "Type: Deprecated Behavior";
// // 		break;
// // 	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
// // 		std::cout << "Type: Undefined Behavior";
// // 		break;
// // 	case GL_DEBUG_TYPE_PORTABILITY:
// // 		std::cout << "Type: Portability";
// // 		break;
// // 	case GL_DEBUG_TYPE_PERFORMANCE:
// // 		std::cout << "Type: Performance";
// // 		break;
// // 	case GL_DEBUG_TYPE_MARKER:
// // 		std::cout << "Type: Marker";
// // 		break;
// // 	case GL_DEBUG_TYPE_PUSH_GROUP:
// // 		std::cout << "Type: Push Group";
// // 		break;
// // 	case GL_DEBUG_TYPE_POP_GROUP:
// // 		std::cout << "Type: Pop Group";
// // 		break;
// // 	case GL_DEBUG_TYPE_OTHER:
// // 		std::cout << "Type: Other";
// // 		break;
// // 	}
// // 	std::cout << std::endl;

// // 	switch (severity)
// // 	{
// // 	case GL_DEBUG_SEVERITY_HIGH:
// // 		std::cout << "Severity: high";
// // 		break;
// // 	case GL_DEBUG_SEVERITY_MEDIUM:
// // 		std::cout << "Severity: medium";
// // 		break;
// // 	case GL_DEBUG_SEVERITY_LOW:
// // 		std::cout << "Severity: low";
// // 		break;
// // 	case GL_DEBUG_SEVERITY_NOTIFICATION:
// // 		std::cout << "Severity: notification";
// // 		break;
// // 	}
// // 	std::cout << std::endl;
// // 	std::cout << std::endl;
// // }

// #include <string>

// void APIENTRY debugCallback(GLenum source, GLenum type, GLuint id,
// 							GLenum severity, GLsizei length,
// 							const GLchar *message, const void *userParam)
// {
// 	(void)length; // Unused parameter
// 	(void)userParam; // Unused parameter

// 	// Ignore non-significant error/warning codes
// 	if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
// 		return;

// 	std::string debugMsg;
// 	debugMsg += "Debug message (" + std::to_string(id) + "): " + message + "\n";

// 	switch (source)
// 	{
// 	case GL_DEBUG_SOURCE_API:
// 		debugMsg += "Source: API\n";
// 		break;
// 	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
// 		debugMsg += "Source: Window System\n";
// 		break;
// 	case GL_DEBUG_SOURCE_SHADER_COMPILER:
// 		debugMsg += "Source: Shader Compiler\n";
// 		break;
// 	case GL_DEBUG_SOURCE_THIRD_PARTY:
// 		debugMsg += "Source: Third Party\n";
// 		break;
// 	case GL_DEBUG_SOURCE_APPLICATION:
// 		debugMsg += "Source: Application\n";
// 		break;
// 	case GL_DEBUG_SOURCE_OTHER:
// 		debugMsg += "Source: Other\n";
// 		break;
// 	}

// 	switch (type)
// 	{
// 	case GL_DEBUG_TYPE_ERROR:
// 		debugMsg += "Type: Error\n";
// 		break;
// 	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
// 		debugMsg += "Type: Deprecated Behavior\n";
// 		break;
// 	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
// 		debugMsg += "Type: Undefined Behavior\n";
// 		break;
// 	case GL_DEBUG_TYPE_PORTABILITY:
// 		debugMsg += "Type: Portability\n";
// 		break;
// 	case GL_DEBUG_TYPE_PERFORMANCE:
// 		debugMsg += "Type: Performance\n";
// 		break;
// 	case GL_DEBUG_TYPE_MARKER:
// 		debugMsg += "Type: Marker\n";
// 		break;
// 	case GL_DEBUG_TYPE_PUSH_GROUP:
// 		debugMsg += "Type: Push Group\n";
// 		break;
// 	case GL_DEBUG_TYPE_POP_GROUP:
// 		debugMsg += "Type: Pop Group\n";
// 		break;
// 	case GL_DEBUG_TYPE_OTHER:
// 		debugMsg += "Type: Other\n";
// 		break;
// 	}

// 	switch (severity)
// 	{
// 	case GL_DEBUG_SEVERITY_HIGH:
// 		debugMsg += "Severity: high\n";
// 		LOG_CRITICAL(debugMsg.c_str());
// 		break;
// 	case GL_DEBUG_SEVERITY_MEDIUM:
// 		debugMsg += "Severity: medium\n";
// 		LOG_ERROR(debugMsg.c_str());
// 		break;
// 	case GL_DEBUG_SEVERITY_LOW:
// 		debugMsg += "Severity: low\n";
// 		LOG_WARN(debugMsg.c_str());
// 		break;
// 	case GL_DEBUG_SEVERITY_NOTIFICATION:
// 		debugMsg += "Severity: notification\n";
// 		LOG_INFO(debugMsg.c_str());
// 		break;
// 	}
// }

// // In your initialization code:
// void setupDebugOutput()
// {
// 	// Make sure you've created a debug context

// 	// During GLFW initialization (if using GLFW):
// 	// glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

// 	// Enable debug output
// 	glEnable(GL_DEBUG_OUTPUT);
// 	// Make the callback be called synchronously (while the error happens)
// 	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
// 	// Register the callback
// 	glDebugMessageCallback(debugCallback, nullptr);

// 	// Control which messages are reported
// 	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

// 	// You can also filter specific messages:
// 	// glDebugMessageControl(GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_ERROR, GL_DONT_CARE, 0, nullptr, GL_TRUE);
// }

// // Main code
// int main(int, char **)
// {
// 	Log::Init();
// 	LOG_TRACE("Trace message");
// 	LOG_DEBUG("Debug message");
// 	LOG_INFO("Info message");
// 	LOG_WARN("Warn message");
// 	LOG_ERROR("Error message");
// 	LOG_CRITICAL("Critical message");

// 	glfwSetErrorCallback(glfw_error_callback);
// 	if (!glfwInit())
// 		return 1;

// 	// GL 3.0 + GLSL 130
// 	const char *glsl_version = "#version 460";
// 	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
// 	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
// 	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
// 	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

// 	// setupDebugCallback();

// 	int width = 2560;
// 	int height = 1440;
// 	// Create window with graphics context
// 	GLFWwindow *window = glfwCreateWindow(width, height, "Dear ImGui GLFW+OpenGL3 example", nullptr, nullptr);
// 	if (window == nullptr)
// 		return 1;
// 	glfwMakeContextCurrent(window);
// 	glfwSwapInterval(1); // Enable vsync

// 	// Initialize GLEW
// 	glewExperimental = GL_TRUE;
// 	if (glewInit() != GLEW_OK)
// 	{
// 		fprintf(stderr, "Failed to initialize GLEW\n");
// 		return -1;
// 	}

// 	setupDebugOutput();
// 	// Setup Dear ImGui context
// 	IMGUI_CHECKVERSION();
// 	ImGui::CreateContext();
// 	ImGuiIO &io = ImGui::GetIO();
// 	(void)io;
// 	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
// 	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

// 	// Setup Dear ImGui style
// 	ImGui::StyleColorsDark();
// 	// ImGui::StyleColorsLight();

// 	// Setup Platform/Renderer backends
// 	ImGui_ImplGlfw_InitForOpenGL(window, true);
// 	ImGui_ImplOpenGL3_Init(glsl_version);

// 	std::shared_ptr<ShaderLibrary> shaderLibrary = std::make_shared<ShaderLibrary>();
// 	shaderLibrary->Add(Shader::Create("GridShader", "Shaders/grid.vert", "Shaders/grid.frag", "Shaders/grid.geom"));
// 	shaderLibrary->Add(Shader::Create("TriangleShader", "Shaders/triangle.vert", "Shaders/triangle.frag"));
// 	GLuint gridVAO;
// 	glGenVertexArrays(1, &gridVAO);
// 	// Define vertices for two separate triangles
// 	float triangleVertices[] = {
// 		// First triangle (centered at x = -1)
// 		-1.5f, -1.0f, -0.5f, // left vertex
// 		-0.5f, 0.0f, -0.5f,	 // right vertex
// 		-1.0f, 0.0f, 0.5f,	 // top vertex

// 		// Second triangle (centered at x = 1)
// 		0.5f, 0.0f, -0.5f, // left vertex
// 		1.5f, 0.0f, -0.5f, // right vertex
// 		1.0f, 0.0f, 0.5f   // top vertex
// 	};

// 	// Define indices for the two triangles
// 	GLuint indices[] = {
// 		0, 1, 2, // First triangle
// 		3, 4, 5	 // Second triangle
// 	};

// 	ShaderStorageBuffer triangle_vertices_buffer;
// 	triangle_vertices_buffer.init(triangleVertices, sizeof(triangleVertices), 5, GL_STATIC_DRAW);

// 	ShaderStorageBuffer triangle_indices_buffer;
// 	triangle_indices_buffer.init(indices, sizeof(indices), 6, GL_STATIC_DRAW);

// 	float testData[1024];
// 	ShaderStorageBuffer aabbBuffer;
// 	// aabbBuffer.init(testData, sizeof(testData), 1, GL_STATIC_DRAW);
// 	aabbBuffer.initEmpty(sizeof(testData), 7, GL_STREAM_DRAW);

// 	std::vector<uint32_t> triangleIds;
// 	triangleIds.push_back(0);
// 	triangleIds.push_back(1);

// 	// const uint32_t indicesOffset = 3;

// 	#if 1
// 	// Triangle VAO and VBO setup
// 	GLuint triangleEBO;
// 	// GLuint  triangleVAO, triangleEBO;
// 	// GLuint triangleVAO, vertexVBO, triangleEBO;
// 	// glGenVertexArrays(1, &triangleVAO);
// 	// glGenBuffers(1, &vertexVBO);
// 	glGenBuffers(1, &triangleEBO);
	
// 	// Bind VAO first
// 	// glBindVertexArray(triangleVAO);
	
// 	// Vertex positions VBO
// 	// glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
// 	// glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);


// 	float new_tri_vertices[] = {
// 		// First triangle (centered at x = -1)
// 		-1.5f, 0.0f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f,
// 		-0.5f, 0.0f, -0.5f,	 1.0f, 0.0f, 1.0f, 1.0f,
// 		-1.0f, 0.0f, 0.5f,	1.0f, 0.0f, 1.0f, 1.0f,

// 		// Second triangle (centered at x = 1)
// 		0.5f, 0.0f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f,
// 		1.5f, 0.0f, -0.5f, 0.2f, 1.0f, 1.0f, 1.0f,
// 		1.0f, 0.0f, 0.5f,  0.2f, 0.3f, 1.0f, 1.0f,
// 	};

// 	// VertexBuffer vertexBuffer(new_tri_vertices, sizeof(new_tri_vertices));
// 	std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>(new_tri_vertices, sizeof(new_tri_vertices));
// 	// std::shaded_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>(triangleVertices, sizeof(triangleVertices), indices, sizeof(indices));
// 	BufferLayout layout = {
// 		{ShaderDataType::Float3, "aPos"},
// 		{ShaderDataType::Float4, "aColor"},
// 	};

// 	//TODO: ADD ASSERT IF NO LAYOUT IS SET
// 	vertexBuffer->SetLayout(layout);
// 	// uint32_t index = 0;
// 	// const  auto &layout = vertexBuffer.GetLayout();
// 	// for (const auto &element : layout)
// 	// {
// 	// 	glEnableVertexAttribArray(index);
// 	// 	glVertexAttribPointer(index, 
// 	// 		element.GetComponentCount(), 
// 	// 		ShaderDataTypeToOpenGLBaseType(element.Type),
// 	// 		element.Normalized ? GL_TRUE : GL_FALSE, 
// 	// 		layout.GetStride(), 
// 	// 		(const void *)element.Offset);
// 	// 	index++;
// 	// }

// 	// VertexArray vertexArray;
// 	std::shared_ptr<VertexArray> vertexArray = std::make_shared<VertexArray>();
// 	vertexArray->AddVertexBuffer(vertexBuffer);

// 	// IndexBuffer indexBuffer(indices, sizeof(indices) / sizeof(uint32_t));
// 	std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>(indices, sizeof(indices) / sizeof(uint32_t));

// 	vertexArray->SetIndexBuffer(indexBuffer);

// 	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleEBO);
// 	// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);



// 	// glVertexAttribPointer(0, 3, GL_INT, GL_FALSE, 3 * sizeof(float), (void *)0);
// 	// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
// 	// glEnableVertexAttribArray(0);
// 	#else
	
// 	VertexBuffer vertexBuffer(triangleVertices, sizeof(triangleVertices));
// 	// std::shaded_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>(triangleVertices, sizeof(triangleVertices), indices, sizeof(indices));
// 	BufferLayout layout = {
// 		{ShaderDataType::Float3, "aPos"},
// 	};

// 	uint32_t index = 0;
// 	for (const auto &element : vertexBuffer.GetLayout())
// 	{
// 		glEnableVertexAttribArray(index);
// 		glVertexAttribPointer(index, element.GetComponentCount(), ShaderDataTypeToOpenGLBaseType(element.Type),
// 							  element.Normalized ? GL_TRUE : GL_FALSE, vertexBuffer.GetLayout().GetStride(), (const void *)element.Offset);
// 		index++;
// 	}

// #endif

// 	// glBindTexture(GL_TEXTURE_3D, 999);
// 	// Unbind VBO and VAO
// 	glBindBuffer(GL_ARRAY_BUFFER, 0);
// 	glBindVertexArray(0);
// 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

// 	shaderLibrary->Add(Shader::Create("BvhShader", "Shaders/bvh.vert", "Shaders/bvh.frag", "Shaders/bvh.geom"));
// 	GLuint vao;
// 	glGenVertexArrays(1, &vao);
// 	glBindVertexArray(vao);

// 	struct bvhNode
// 	{
// 		float minX, minY, minZ;	   // 12 bytes - bounding box min point
// 		float maxX, maxY, maxZ;	   // 12 bytes - bounding box max point
// 		int leftChildOrPrimOffset; // 4 bytes - child node index or triangleId offset if negative
// 		int rightChildOrPrimCount; // 4 bytes - right child index or primitive count when leftChildOrPrimOffset < 0
// 	};

// 	struct BVHNode
// 	{
// 		float minX, minY, minZ;
// 		float maxX, maxY, maxZ;
// 		int leftChildOrPrimOffset;
// 		int rightChildOrPrimCount;
// 	};

// 	// Create an array of BVHNode structs
// 	BVHNode bvhNodes[] = {
// 		{-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1, 2},
// 		{-2.0f, -2.0f, -2.0f, 0.0f, 1.0f, 1.0f, -1, -1},
// 		{-1.0f, 5.0f, 0.0f, 3.0f, 1.0f, 1.0f, -1, -1}};
// 	// Use the struct array to create the buffer

// 	ShaderStorageBuffer bvhBuffer;
// 	bvhBuffer.init(bvhNodes, sizeof(bvhNodes), 0, GL_STATIC_DRAW);
// 	// GLuint bvhBuffer;
// 	// glGenBuffers(1, &bvhBuffer);
// 	// glBindBuffer(GL_SHADER_STORAGE_BUFFER, bvhBuffer);
// 	// glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(bvhNodes), bvhNodes, GL_STATIC_DRAW);
// 	// glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, bvhBuffer);

// 	// glObjectLabel(GL_BUFFER, bvhBuffer, -1, "BVH_Data_Buffer");

// 	// Grid parameters
// 	float gridSize = 10.0f;
// 	float gridSpacing = 1.0f;
// 	bool showGrid = true;

// 	// Triangle parameters
// 	bool showTriangle = true;
// 	glm::vec3 trianglePosition(0.0f, 0.0f, 0.0f);
// 	float triangleRotation = 0.0f;
// 	float triangleScale = 1.0f;

// 	// Enable depth testing
// 	glEnable(GL_DEPTH_TEST);

// 	// Enable blending for transparent grid lines
// 	glEnable(GL_BLEND);
// 	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

// 	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

// 	Camera camera(width, height, glm::vec3(0.0f, 2.0f, 2.0f));

// 	glm::vec3 triangleColor(1.0f, 0.0f, 0.0f); // Red by default

// 	float previousTime = static_cast<float>(glfwGetTime());
// 	float deltaTime = 0.0f;

// 	ComputeShader computeShader;
// 	computeShader.LoadFromFile("Shaders/aabb.compute");
// 	// Main loop
// 	while (!glfwWindowShouldClose(window))
// 	{
// 		// Calculate delta time
// 		float currentTime = static_cast<float>(glfwGetTime());
// 		deltaTime = currentTime - previousTime;
// 		previousTime = currentTime;

// 		// Poll and handle events (inputs, window resize, etc.)
// 		glfwPollEvents();
// 		if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
// 		{
// 			ImGui_ImplGlfw_Sleep(10);
// 			continue;
// 		}

// 		// Start the Dear ImGui frame
// 		ImGui_ImplOpenGL3_NewFrame();
// 		ImGui_ImplGlfw_NewFrame();
// 		ImGui::NewFrame();

// 		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
// 		{
// 			static float f = 0.0f;
// 			static int counter = 0;

// 			ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.

// 			ImGui::Text("This is some useful text."); // Display some text (you can use a format strings too)
// 			ImGui::Checkbox("Show Grid", &showGrid);  // Toggle grid visibility

// 			ImGui::SliderFloat("Grid Size", &gridSize, 5.0f, 20.0f);	  // Control grid size
// 			ImGui::SliderFloat("Grid Spacing", &gridSpacing, 0.5f, 2.0f); // Control grid spacing

// 			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);			 // Edit 1 float using a slider from 0.0f to 1.0f
// 			ImGui::ColorEdit3("clear color", (float *)&clear_color); // Edit 3 floats representing a color

// 			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

// 			// Add to the ImGui controls section
// 			ImGui::Text("Triangle Controls");
// 			ImGui::DragFloat3("Triangle Position", glm::value_ptr(trianglePosition), 0.1f);
// 			ImGui::SliderFloat("Triangle Rotation", &triangleRotation, 0.0f, 360.0f);
// 			ImGui::SliderFloat("Triangle Scale", &triangleScale, 0.1f, 5.0f);
// 			ImGui::ColorEdit3("Triangle Color", glm::value_ptr(triangleColor)); // Add color picker

// 			// ImGui::DragFloat3("Camera Position", glm::type_ptr(camera.GetPosition())); // Control camera position
// 			glm::vec3 position = camera.GetPosition();						// Copy the position
// 			ImGui::DragFloat3("Camera Position", glm::value_ptr(position)); // Modify the local copy
// 			camera.SetPosition(position);									// Update the camera with the modified position
// 			ImGui::End();
// 		}

// 		// Rendering
// 		ImGui::Render();
// 		int display_w, display_h;
// 		glfwGetFramebufferSize(window, &display_w, &display_h);
// 		glViewport(0, 0, display_w, display_h);
// 		// glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
// 		glClearColor(0.0f, 0.078f, 0.118f, 1.0f);
// 		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

// 		computeShader.Use();
// 		computeShader.SetInt("numTriangles", 2);
// 		computeShader.Dispatch(64, 1, 1); // Dispatch the compute shader with 1 work group in each dimension

// 		if (!ImGui::GetIO().WantCaptureMouse && !ImGui::GetIO().WantCaptureKeyboard)
// 		{
// 			camera.Inputs(window, deltaTime);
// 		}

// 		// Render the grid if enabled
// 		if (showGrid)
// 		{
// 			// auto gridShader = shaderLibrary->Get("GridShader");
// 			std::shared_ptr<Shader> gridShader = shaderLibrary->Get("GridShader");
// 			gridShader->Bind();

// 			gridShader->SetMat4("viewProj", camera.Matrix(45.0f, 0.1f, 100.0f));
// 			gridShader->SetFloat("gridSize", gridSize);
// 			gridShader->SetFloat("gridSpacing", gridSpacing);

// 			glBindVertexArray(gridVAO);
// 			glDrawArrays(GL_POINTS, 0, 1); // Draw a single point, geometry shader creates the grid
// 			glBindVertexArray(0);
// 		}

// 		// In the rendering section
// 		if (showTriangle)
// 		{
// 			std::shared_ptr<Shader> triangleShader = shaderLibrary->Get("TriangleShader");
// 			triangleShader->Bind();

// 			// Calculate model matrix for triangle
// 			glm::mat4 model = glm::mat4(1.0f);
// 			model = glm::translate(model, trianglePosition);
// 			model = glm::rotate(model, glm::radians(triangleRotation), glm::vec3(0.0f, 1.0f, 0.0f));
// 			model = glm::scale(model, glm::vec3(triangleScale));

// 			// Set shader uniforms
// 			triangleShader->SetMat4("viewProj", camera.Matrix(45.0f, 0.1f, 100.0f));
// 			triangleShader->SetMat4("model", model);
// 			// triangleShader->SetFloat3("triangleColor", triangleColor); // Set the color uniform

// 			// Draw triangle
// 			// glBindVertexArray(triangleVAO);
// 			vertexArray->Bind();
// 			// glDrawArrays(GL_TRIANGLES, 0, 3);
// 			// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // Draw the triangle using indices
// 			glDrawElements(GL_TRIANGLES, indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr); // Draw the triangle using indices
// 			vertexArray->Unbind();
// 			// glDrawArrays(GL_TRIANGLES, 0, 6);
// 			// glBindVertexArray(0);
// 			// vertexBuffer.Bind();

// 		}

// 		// Draw BVH nodes
// 		{
// 			std::shared_ptr<Shader> bvhShader = shaderLibrary->Get("BvhShader");
// 			bvhShader->Bind();

// 			// Set the view projection matrix
// 			bvhShader->SetMat4("viewProj", camera.Matrix(45.0f, 0.1f, 100.0f));

// 			// Draw the BVH nodes
// 			glBindVertexArray(vao);
// 			glDrawArrays(GL_POINTS, 0, 1); // Adjust the count as needed
// 			glBindVertexArray(0);
// 		}

// 		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

// 		glfwSwapBuffers(window);
// 	}

// 	// Cleanup
// 	glDeleteVertexArrays(1, &gridVAO);
// 	// glDeleteProgram(gridProgram);

// 	ImGui_ImplOpenGL3_Shutdown();
// 	ImGui_ImplGlfw_Shutdown();
// 	ImGui::DestroyContext();

// 	glfwDestroyWindow(window);
// 	glfwTerminate();

// 	return 0;
// }