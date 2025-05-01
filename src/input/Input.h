#pragma once

#include <iostream>
#include <utility>

#include "KeyCodes.h"
#include "MouseButtonCodes.h"

class Input
{
public:
	static bool IsKeyPressed(Key keycode);
	static bool IsMouseButtonPressed(MouseButton button);
	static std::pair<float, float> GetMousePosition();

	static void SetCursorMode(CursorMode mode);
	static void SetCursorPosition(float x, float y);
};
