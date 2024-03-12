#include "Window.hpp"
#include "ImGUI/UI.hpp"

namespace IW3SR::Engine
{
	Window::Window(const std::string& name)
	{
		Name = name;
	}

	void Window::SetDesigner(bool state)
	{
		Designer = state;
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

		ImGui::Begin(Name.c_str(), &Open, flags | ImGuiWindowFlags_NoCollapse);

		if (ImGui::IsWindowChanged())
		{
			vec2 framePosition = ImGui::GetWindowPos();
			vec2 frameSize = ImGui::GetWindowSize();

			x = framePosition.x;
			y = framePosition.y;
			w = frameSize.x;
			h = frameSize.y;

			UI::Get().Screen.Reverse(x, y, w, h, HorizontalAlign, VerticalAlign);
			Position = { x, y };
			Size = { w, h };
		}
		else
		{
			ImGui::SetWindowPos({ x, y });
			ImGui::SetWindowSize({ w, h });
		}
		if (Designer)
			ImGui::Movable(ID, Position, Size, RenderPosition, RenderSize);
	}

	void Window::Menu(const std::string& label, bool open)
	{
		if (!ImGui::CollapsingHeader(label, open))
			return;

		ImGui::PushID(label.c_str());

		ImGui::DragFloat2("Position", Position);
		ImGui::DragFloat2("Size", Size);
		ImGui::ComboAlignRect(&HorizontalAlign, &VerticalAlign);

		ImGui::PopID();
	}

	void Window::End()
	{
		ImGui::End();
	}
}
