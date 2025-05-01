#include "Input.h"

#include "Application.h"

#include <GLFW/glfw3.h>

bool Input::IsKeyPressed(Key keycode)
{
	return (glfwGetKey(Application::Get().GetWindow().GetNativeWindow(), static_cast<int>(keycode)) == GLFW_PRESS);
}

bool Input::IsMouseButtonPressed(MouseButton button)
{
	return (glfwGetMouseButton(Application::Get().GetWindow().GetNativeWindow(), static_cast<int>(button)) == GLFW_PRESS);
}

std::pair<float, float> Input::GetMousePosition()
{
	double xpos, ypos;
	glfwGetCursorPos(Application::Get().GetWindow().GetNativeWindow(), &xpos, &ypos);
	return {(float)xpos, (float)ypos};
}

void Input::SetCursorMode(CursorMode mode)
{
	return glfwSetInputMode(Application::Get().GetWindow().GetNativeWindow(), GLFW_CURSOR, static_cast<int>(mode));
}

void Input::SetCursorPosition(float x, float y)
{
	return glfwSetCursorPos(Application::Get().GetWindow().GetNativeWindow(), x, y);
}
