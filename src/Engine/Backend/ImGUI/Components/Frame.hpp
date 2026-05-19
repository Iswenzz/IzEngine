#pragma once
#include "Engine/Backend/ImGUI/Base.hpp"

namespace IzEngine
{
	class API Frame : public IObject
	{
	public:
		std::string Name = UUID();
		vec2 Position = { 20, 20 };
		vec2 Size = { 250, 140 };
		vec2 RenderPosition;
		vec2 RenderSize;
		bool Open = false;
		bool Designer = false;

		ImGuiWindowFlags Flags = ImGuiWindowFlags_NoCollapse;
		Horizontal HorizontalAlign = Horizontal::Left;
		Vertical VerticalAlign = Vertical::Top;

		Frame() = default;
		Frame(const std::string& name);
		virtual ~Frame();

		virtual void Initialize();
		virtual void Release();

		virtual void SetRect(const vec2& position, const vec2& size);
		virtual void SetRect(float x, float y, float w, float h);
		virtual void SetRectAlignment(Horizontal horizontal, Vertical vertical);
		virtual void SetDesigner(bool state);
		virtual void SetFlags(ImGuiWindowFlags flags);

		virtual void Menu(const std::string& label, bool open = false);
		virtual void Begin();
		virtual void End();

		virtual void OnRender();
		virtual void OnEvent(Event& event);

		SERIALIZE_POLY_BASE(Frame, Position, Size, Open, HorizontalAlign, VerticalAlign)
	};
}
