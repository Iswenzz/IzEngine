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

	// Keeps the frame inside the screen. A window dragged past an edge is unreachable afterwards,
	// and a saved layout outlives the resolution it was made at.
	bool Frame::Bound(vec2& position, vec2& size) const
	{
		const vec2 screen = UI::Screen.Size;
		if (!Bounded || screen.x <= 0.0f || screen.y <= 0.0f)
			return false;

		const vec2 bounded = glm::min(size, screen);
		const vec2 anchor = glm::clamp(position, vec2(0.0f), screen - bounded);

		const bool changed = anchor != position || bounded != size;
		position = anchor;
		size = bounded;

		return changed;
	}

	// Takes a rect back out of screen space into the aligned one the layout is saved in.
	void Frame::Store(vec2 position, vec2 size)
	{
		UI::Screen.Reverse(position, size, HorizontalAlign, VerticalAlign);
		Position = position;
		Size = size;
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

		// Placing the window before Begin, decorations included. Correcting it afterwards only moves
		// the contents: the title bar and the background are already in the draw list by then.
		ImGuiWindow* window = ImGui::FindWindowByName(Name.c_str());
		const bool interacting = ImGui::IsInteracting(window);

		// A drag is applied to the window before Begin, so the rect about to be drawn is read from
		// there rather than from the stored layout, bounded, and handed straight back.
		if (interacting)
		{
			position = window->Pos;
			size = window->SizeFull;
		}
		if (Bound(position, size) || interacting)
			Store(position, size);

		ImGui::SetNextWindowPos(position);
		if (!interacting)
			ImGui::SetNextWindowSize(size);

		// A resize grip is only handled inside Begin, past any size set here, so the size it may
		// reach is capped instead.
		if (Bounded && UI::Screen.Size.x > 0.0f && UI::Screen.Size.y > 0.0f)
			ImGui::SetNextWindowSizeConstraints(vec2(0.0f), UI::Screen.Size);

		RenderPosition = position;
		RenderSize = size;

		ImGui::Begin(Name.c_str(), &Open, Flags);
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
