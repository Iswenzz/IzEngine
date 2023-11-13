#include "Text.hpp"
#include "Draw2D.hpp"

#include "Sys/Log.hpp"

namespace IW3SR
{
	Text::Text(const std::string& text, const std::string& font, float x, float y, float size, const vec4& color)
	{
		float fontSize = scr_place->scaleVirtualToReal[0] * size * 14.f;

		Value = text;
		Position = { x, y };
		Size = { fontSize, fontSize };
		Color = color;
		Font = nullptr;
		FontName = font;
		FontIndex = 0;
	}

	Text::Text(const std::string& text, const std::string& font, const vec2& pos, float size, const vec4& color) :
		Text(text, font, pos.x, pos.y, size, color) { }

	void Text::SetRectAlignment(RectAlignHorizontal_t horizontal, RectAlignVertical_t vertical)
	{
		HorizontalAlign = horizontal;
		VerticalAlign = vertical;
	}

	void Text::SetAlignment(hudalign_t horizontal, hudalign_t vertical)
	{
		AlignX = horizontal;
		AlignY = vertical;
	}

	void Text::SetFont(const std::string& font)
	{
		if (Font) Assets::UnloadFont(Font);
		Font = Assets::LoadFont(font, Size.y);
		FontName = font;
		FontIndex = std::distance(Assets::FontNames.begin(), std::ranges::find(Assets::FontNames, font));
	}

	void Text::ComputeAlignment(float& x, float& y)
	{
		RECT textRect = { 0, 0, 0, 0 };
		Font->DrawTextA(NULL, Value.c_str(), -1, &textRect, DT_CALCRECT, 0);

		int textWidth = textRect.right - textRect.left;
		int textHeight = textRect.bottom - textRect.top;

		if (AlignX & HUDALIGN_CENTER)
			x += -(textWidth / 2.f);
		else if (AlignX & HUDALIGN_RIGHT)
			x += -textWidth;

		if (AlignY & HUDALIGN_MIDDLE)
			y += textHeight / 2.f;
		else if (AlignY & HUDALIGN_BOTTOM)
			y += textHeight;
	}

	void Text::Render()
	{
		float x = Position.x;
		float y = Position.y;
		float w = Size.x;
		float h = Size.y;

		if (!Font)
			SetFont(FontName);

		// @TODO
		Log::WriteLine("{} {}", w, h);

		ComputeAlignment(x, y);
		Math::ApplyRect(x, y, w, h, HorizontalAlign, VerticalAlign);
		RECT rect = { static_cast<int>(x), static_cast<int>(y), 0, 0 };
		Font->DrawTextA(NULL, Value.c_str(), -1, &rect, DT_NOCLIP, Color);
	}
}
