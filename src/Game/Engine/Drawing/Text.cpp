#include "Text.hpp"
#include "Draw2D.hpp"

namespace IW3SR::Engine
{
	Text::Text(const std::string& text, const std::string& font, float x, float y, float size, const vec4& color)
	{
		Value = text;
		Position = { x, y };
		Color = color;
		Font = nullptr;
		FontName = font;
		FontSize = size;
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
		Font = Assets::Fonts[font];
		FontName = font;
		FontIndex = std::distance(Assets::FontNames.begin(), std::ranges::find(Assets::FontNames, font));
	}

	void Text::ComputeAlignment(float& x, float& y)
	{
		if (AlignX & HUDALIGN_CENTER)
			x += -(Size.x / 2.f);
		else if (AlignX & HUDALIGN_RIGHT)
			x += -Size.x;

		if (AlignY & HUDALIGN_MIDDLE)
			y += Size.y / 2.f;
		else if (AlignY & HUDALIGN_BOTTOM)
			y += Size.y;
	}

	void Text::Render()
	{
		float x = Position.x;
		float y = Position.y;

		if (!Font)
			SetFont(FontName);

		Size = { R_TextWidth(Value.c_str(), Value.size(), Font) * FontSize, Font->pixelHeight * FontSize };

		ComputeAlignment(x, y);
		Math::ApplyRect(x, y, HorizontalAlign, VerticalAlign);
		R_AddCmdDrawText(Value.c_str(), 0x7FFFFFFF, Font, x, y, FontSize, FontSize, 0, 0, Color);
	}
}
