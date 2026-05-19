#include "Window.hpp"

namespace IzEngine
{
	void Window::SetCapture(bool state)
	{
		IsCapture = state;
	}

	bool Window::IsStyle(int value)
	{
		return GetStyle() == value;
	}

	bool Window::HasStyle(int value)
	{
		return GetStyle() & value;
	}
}
