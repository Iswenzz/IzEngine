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
		const vec2 space = vec2(scr_place->scaleVirtualToFull);
		RenderPosition = Position * space;
		RenderSize = Size * space;

		ImGui::SetNextWindowPos(RenderPosition, ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(RenderSize, ImGuiCond_FirstUseEver);

		ImGui::Begin(Name.c_str(), &Open, flags);
		RenderPosition = ImGui::GetWindowPos();
		RenderSize = ImGui::GetWindowSize();

		Position = RenderPosition / space;
		Size = RenderSize / space;

		if (!(flags & (ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)))
		{
			ImGui::Movable("#" + Name, Position, Size, RenderPosition, RenderSize);
			ImGui::SetWindowPos(RenderPosition);
			ImGui::SetWindowSize(RenderSize);
		}
	}

	void Window::End()
	{
		ImGui::End();
	}
}
