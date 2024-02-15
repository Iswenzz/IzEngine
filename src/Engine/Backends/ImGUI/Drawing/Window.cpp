#include "Window.hpp"
#include "Engine/Backends/ImGUI/UI.hpp"

namespace IW3SR::Engine
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

	void Window::SetRectAlignment(Horizontal horizontal, Vertical vertical)
	{
		HorizontalAlign = horizontal;
		VerticalAlign = vertical;
	}

	void Window::Begin(ImGuiWindowFlags flags)
	{
		float x = Position.x;
		float y = Position.y;
		float w = Size.x;
		float h = Size.y;

		UI::Get().Screen.Apply(x, y, w, h, HorizontalAlign, VerticalAlign);
		RenderPosition = { x, y };
		RenderSize = { w, h };

		ImGui::SetNextWindowPos(RenderPosition, ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(RenderSize, ImGuiCond_FirstUseEver);

		ImGui::PushID(ID.c_str());
		ImGui::Begin(Name.c_str(), &Open, flags | ImGuiWindowFlags_NoCollapse);
		RenderPosition = ImGui::GetWindowPos();
		RenderSize = ImGui::GetWindowSize();

		x = RenderPosition.x;
		y = RenderPosition.y;
		w = RenderSize.x;
		h = RenderSize.y;

		UI::Get().Screen.Reverse(x, y, w, h, HorizontalAlign, VerticalAlign);
		Position = { x, y };
		Size = { w, h };

		if (!(flags & ImGuiWindowFlags_NoMove))
			ImGui::Movable("#" + ID, Position, Size, RenderPosition, RenderSize);

		ImGui::SetWindowPos(RenderPosition);
		ImGui::SetWindowSize(RenderSize);
	}

	void Window::Menu(const std::string& label, bool open)
	{
		if (!ImGui::CollapsingHeader(label.c_str(), open))
			return;

		ImGui::PushID(label.c_str());

		ImGui::DragFloat2("Position", Position);
		ImGui::DragFloat2("Size", Size);

		ImGui::PopID();
	}

	void Window::End()
	{
		ImGui::End();
		ImGui::PopID();
	}
}
