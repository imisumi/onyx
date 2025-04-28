#pragma once

#include <string>
#include <string_view>
#include <functional>
#include <memory>

// Forward declaration
struct GLFWwindow;

class Window
{
public:
	Window(std::string_view title = "Default title", uint32_t width = 2560, uint32_t height = 1440, bool vsync = false);
	~Window() {};

	GLFWwindow *GetNativeWindow() { return m_Window; }
	const char *GetGLSLVersion() { return m_GLSLVersion.c_str(); }

	void PollEvents();
	void SwapBuffers();
	bool ShouldClose();

	void OnResize(int width, int height);

	unsigned int GetWidth() { return m_Width; }
	unsigned int GetHeight() { return m_Height; }

	// Window attributes
	void SetVSync(bool enabled);
	bool IsVSync() const { return m_VSync; }
	void SetTitle(std::string_view title) { m_Title = title; }

	static std::unique_ptr<Window> Create(std::string_view title = "Default title", uint32_t width = 2560, uint32_t height = 1440, bool vsync = false)
	{
		return std::make_unique<Window>(title, width, height, vsync);
	}

private:
	void Init();
	void Shutdown();

private:
	GLFWwindow *m_Window; // Forward declared type

	std::string m_GLSLVersion;

	std::string m_Title;
	unsigned int m_Width, m_Height;
	bool m_VSync;
};