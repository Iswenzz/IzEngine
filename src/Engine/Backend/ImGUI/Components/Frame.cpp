#include "Frame.hpp"

#include "Engine/Backend/ImGUI/Common.hpp"

namespace IzEngine
{
	Frame::Frame(const std::string& name)
	{
		Name = name;
	}

	Frame::~Frame()
	{
		Release();
	}

	void Frame::Initialize() { }
	void Frame::Release() { }

	void Frame::SetRect(const vec2& position, const vec2& size)
	{
		SetRect(position.x, position.y, size.x, size.y);
	}

	void Frame::SetRect(float x, float y, float w, float h)
	{
		Position = { x, y };
		Size = { w, h };
	}

	void Frame::SetRectAlignment(Horizontal horizontal, Vertical vertical)
	{
		HorizontalAlign = horizontal;
		VerticalAlign = vertical;
	}

	void Frame::SetDesigner(bool state)
	{
		Designer = state;
	}

	void Frame::SetFlags(ImGuiWindowFlags flags)
	{
		Flags = flags;
	}

	void Frame::Menu(const std::string& label, bool open)
	{
		if (!ImGui::CollapsingHeader(label, open))
			return;

		ImGui::PushID(label.c_str());

		ImGui::DragFloat2("Position", &Position.x);
		ImGui::DragFloat2("Size", &Size.x);
		ImGui::ComboAlignRect(&HorizontalAlign, &VerticalAlign);

		ImGui::PopID();
	}

	void Frame::Begin()
	{
		vec2 position = Position;
		vec2 size = Size;

		UI::Screen.Apply(position, size, HorizontalAlign, VerticalAlign);
		RenderPosition = position;
		RenderSize = size;

		ImGui::Begin(Name.c_str(), &Open, Flags);

		if (ImGui::IsMoving())
		{
			position = ImGui::GetWindowPos();
			size = ImGui::GetWindowSize();

			UI::Screen.Reverse(position, size, HorizontalAlign, VerticalAlign);
			Position = position;
			Size = size;
		}
		else
		{
			ImGui::SetWindowPos(position);
			ImGui::SetWindowSize(size);
		}
		if (Designer)
			ImGui::Movable(ID, Position, Size, RenderPosition, RenderSize);
	}

	void Frame::End()
	{
		ImGui::End();
	}

	void Frame::OnRender() { }

	void Frame::OnEvent(Event& event)
	{
		if (!UI::Open || !Open)
			return;

		EventDispatcher dispatcher(event);

		dispatcher.Dispatch<EventRendererRender>(EVENT_BIND_VOID(OnRender));
	}
}
