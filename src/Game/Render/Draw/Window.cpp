#include "Window.hpp"

namespace IW3SR
{
	Window::Window(std::string name)
	{
		Name = name;
	}

	void Window::SetRect(const vec2& position, const vec2& size)
	{
		SetRect(position.x, position.y, size.x, size.y);
	}

	void Window::SetRect(float x, float y, float w, float h)
	{
		Position = { x, y };
		Size = { w, h };
	}

	void Window::Begin(ImGuiWindowFlags flags)
	{
		ImGui::Begin(Name.c_str(), &Open, flags);

		const vec2 space = vec2(scr_place->scaleVirtualToFull);
		vec2 position = Position * space;
		vec2 size = Size * space;

		ImGui::SetWindowPos(position, ImGuiCond_FirstUseEver);
		ImGui::SetWindowSize(size, ImGuiCond_FirstUseEver);

		Position = vec2(ImGui::GetWindowPos()) / space;
		Size = vec2(ImGui::GetWindowSize()) / space;
	}

	void Window::End()
	{
		ImGui::End();
	}
}
