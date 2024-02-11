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

	void Window::Begin(ImGuiWindowFlags flags)
	{
		const vec2 space = UI::Get().Screen.VirtualToFull;
		RenderPosition = Position * space;
		RenderSize = Size * space;

		ImGui::SetNextWindowPos(RenderPosition, ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(RenderSize, ImGuiCond_FirstUseEver);

		ImGui::PushID(ID.c_str());
		ImGui::Begin(Name.c_str(), &Open, flags | ImGuiWindowFlags_NoCollapse);
		RenderPosition = ImGui::GetWindowPos();
		RenderSize = ImGui::GetWindowSize();

		Position = RenderPosition / space;
		Size = RenderSize / space;

		if (!(flags & (ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)))
			ImGui::Movable("#" + Name, Position, Size, RenderPosition, RenderSize);

		ImGui::SetWindowPos(RenderPosition);
		ImGui::SetWindowSize(RenderSize);
	}

	void Window::Menu(const std::string& label, bool open)
	{
		if (!ImGui::CollapsingHeader(label.c_str(), open))
			return;

		ImGui::PushID(ID.c_str());

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
