#include "Text.hpp"

namespace IW3SR
{
	Text::Text(const std::string& text, const std::string& font, float x, float y, float size, const vec4& color) :
		HUD("white", x, y, size, size, color)
	{
		Value = text;
		Font = nullptr;
		FontName = font;
	}

	Text::Text(const std::string& text, const std::string& font, const vec2& pos, float size, const vec4& color) :
		Text(text, font, pos.x, pos.y, size, color) { }

	void Text::ComputeAlignment(float& x, float& y)
	{
		float textWidth = R_TextWidth(Value.c_str(), Value.size(), Font) * Size.x;
		float textHeight = Font->pixelHeight * Size.y;

		if (AlignX & HUDALIGN_CENTER)
			x += -(textWidth / 2);
		else if (AlignX & HUDALIGN_RIGHT)
			x += textWidth;

		if (AlignY & HUDALIGN_MIDDLE)
			y += textHeight / 2;
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
			Font = R_RegisterFont(FontName.c_str(), FontName.size());

		ComputeAlignment(x, y);
		ScrPlace_ApplyRect(x, y, w, h, HorizontalAlign, VerticalAlign);
		R_AddCmdDrawText(Value.c_str(), 0x7FFFFFFF, Font, x, y, w, h, 0, 0, Color);
	}
}
