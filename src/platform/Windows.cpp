#include "Window.h"

#include "core/Base.h"

#include <GL/glew.h> // Include GLEW before GLFW
#include <GLFW/glfw3.h>

static bool s_GLFWInitialized = false;

static void glfw_error_callback(int error, const char *description)
{
	// fprintf(stderr, "GLFW Error %d: %s\n", error, description);
	LOG_ERROR("GLFW Error {0}: {1}", error, description);
}

static void APIENTRY debugCallback(GLenum source, GLenum type, GLuint id,
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
static void setupDebugOutput()
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

Window::Window(std::string_view title, uint32_t width, uint32_t height, bool vsync)
	: m_Title(title), m_Width(width), m_Height(height), m_VSync(vsync)
{
	Init();
}

void Window::Shutdown()
{
	glfwDestroyWindow(m_Window);
	glfwTerminate();
}

void Window::Init()
{
	LOG_INFO("Creating window {0} ({1}, {2})", m_Title, m_Width, m_Height);

	if (!s_GLFWInitialized)
	{
		glfwSetErrorCallback(glfw_error_callback);
		int success = glfwInit();
		MY_ASSERT(success, "Could not initialize GLFW!");
		s_GLFWInitialized = true;
	}

	// const char *glsl_version = "#version 460";
	m_GLSLVersion = "#version 460";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);
	if (!m_Window)
	{
		MY_ASSERT(false, "Failed to create GLFW window!");
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(m_Window);
	SetVSync(m_VSync);

	//? init glew
	glewExperimental = GL_TRUE; // Needed for core profile
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		LOG_ERROR("Failed to initialize GLEW: {}", (const char *)glewGetErrorString(err));
		glfwDestroyWindow(m_Window);
		glfwTerminate();
		return;
	}

	setupDebugOutput();

	LOG_INFO("OpenGL version: {0}", (const char *)glGetString(GL_VERSION));

	// TODO: have api for this
	glEnable(GL_DEPTH_TEST);

	// Enable blending for transparent grid lines
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//? Set resize callback
	glfwSetWindowUserPointer(m_Window, this);
	glfwSetWindowSizeCallback(m_Window, [](GLFWwindow *window, int width, int height)
							  {
		Window *win = static_cast<Window *>(glfwGetWindowUserPointer(window));
		win->OnResize(width, height); });
}

// void Window::OnUpdate()
// {
// 	glfwPollEvents();
// 	glfwSwapBuffers(m_Window);
// }

void Window::PollEvents()
{
	glfwPollEvents();
}

void Window::SwapBuffers()
{
	glfwSwapBuffers(m_Window);
}

bool Window::ShouldClose()
{
	return glfwWindowShouldClose(m_Window);
}

void Window::SetVSync(bool enabled)
{
	glfwSwapInterval(enabled ? 1 : 0);
	m_VSync = enabled;
}

void Window::OnResize(int width, int height)
{
	m_Width = width;
	m_Height = height;
	glViewport(0, 0, width, height);
	glfwSetWindowSize(m_Window, width, height);
}