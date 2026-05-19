#pragma once
#include "Engine/Core/Common.hpp"

#include "Engine/Renderer/Base/Font.hpp"

namespace IzEngine
{
	class API Text : public IObject
	{
	public:
		std::string Value;
		vec2 Position;
		vec2 Size;
		vec2 RenderPosition;
		vec2 RenderSize;
		vec2 Skew;
		vec4 Color;

		Horizontal HorizontalAlign = Horizontal::Left;
		Vertical VerticalAlign = Vertical::Top;
		Alignment AlignX = Alignment::Left;
		Alignment AlignY = Alignment::Top;

		std::string FontName;
		float FontRescale = 13.33f;
		float FontSize = 1.4;
		int FontIndex = 0;

		Text() = default;
		Text(const std::string& text, const std::string& font, float x, float y, float size, const vec4& color);
		virtual ~Text() = default;

		void SetRectAlignment(Horizontal horizontal, Vertical vertical);
		void SetAlignment(Alignment horizontal, Alignment vertical);
		void SetFont(const std::string& font);

		void Menu(const std::string& label, bool open = false);
		void Render();

	private:
		Ref<Font> Font = nullptr;

		void ComputeAlignment(vec2& position);

		SERIALIZE_POLY_BASE(Text, Value, Position, Skew, Color, HorizontalAlign, VerticalAlign, AlignX, AlignY,
			FontName, FontSize)
	};
}
