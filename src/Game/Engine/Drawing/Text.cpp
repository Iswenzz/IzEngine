#include "Text.hpp"
#include "Draw2D.hpp"

#include "Engine/Backends/ImGUI/Components.hpp"

#define RESCALE 0.4f

namespace IW3SR::Game
{
	Text::Text(const std::string& text, const std::string& font, float x, float y, float size, const vec4& color)
	{
		Value = text;
		Position = { x, y };
		Color = color;
		
		SetFont(font);
	}

	void Text::SetRectAlignment(RectAlignHorizontal horizontal, RectAlignVertical vertical)
	{
		HorizontalAlign = horizontal;
		VerticalAlign = vertical;
	}

	void Text::SetAlignment(Alignment horizontal, Alignment vertical)
	{
		AlignX = horizontal;
		AlignY = vertical;
	}

	void Text::SetFont(const std::string& font)
	{
		Font = Assets::Fonts[font];
		FontName = font;

		if (!Font)
		{
			FontName = FONT_OBJECTIVE;
			Font = Assets::Fonts[FontName];
		}
		FontIndex = std::distance(Assets::FontNames.begin(), std::ranges::find(Assets::FontNames, FontName));
	}

	void Text::ComputeAlignment(float& x, float& y)
	{
		if (AlignX & ALIGN_CENTER)
			x += -(Size.x / 2.f);
		else if (AlignX & ALIGN_RIGHT)
			x += -Size.x;

		if (AlignY & ALIGN_MIDDLE)
			y += Size.y / 2.f;
		else if (AlignY & ALIGN_BOTTOM)
			y += Size.y;
	}

	void Text::Menu(const std::string& label, bool open)
	{
		if (!ImGui::CollapsingHeader(label.c_str(), open ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None))
			return;

		const std::vector<std::string>& horizontals = Draw2D::HorizontalAlignment;
		const std::vector<std::string>& verticals = Draw2D::VerticalAlignment;
		const std::vector<std::string>& fonts = Assets::FontNames;

		ImGui::DragFloat2("Position", Position);
		ImGui::ColorEdit4("Color", Color, ImGuiColorEditFlags_Float);

		if (ImGui::InputFloat("Font Size", &FontSize, 0.1))
			SetFont(FontName);
		if (ImGui::Combo("Font", &FontIndex, fonts))
			SetFont(fonts[FontIndex]);

		int horizontal = HorizontalAlign - 1;
		if (ImGui::Combo("Horizontal Alignment", &horizontal, horizontals))
			HorizontalAlign = static_cast<RectAlignHorizontal>(horizontal + 1);

		int vertical = VerticalAlign - 1;
		if (ImGui::Combo("Vertical Alignment", &vertical, verticals))
			VerticalAlign = static_cast<RectAlignVertical>(vertical + 1);

		int alignX = AlignX / 4;
		if (ImGui::Combo("Align X", &alignX, horizontals))
			AlignX = static_cast<Alignment>(alignX * 4);

		int alignY = AlignY;
		if (ImGui::Combo("Align Y", &alignY, verticals))
			AlignY = static_cast<Alignment>(alignY);
	}

	void Text::Render()
	{
		float x = Position.x;
		float y = Position.y;
		float xScale = FontSize * RESCALE;
		float yScale = FontSize * RESCALE;

		Size = { R_TextWidth(Value.c_str(), Value.size(), Font) * xScale, Font->pixelHeight * yScale };

		ComputeAlignment(x, y);
		Math::ApplyRect(x, y, xScale, yScale, HorizontalAlign, VerticalAlign);
		R_AddCmdDrawText(Value.c_str(), 0x7FFFFFFF, Font, x, y, xScale, yScale, 0, 0, Color);
	}
}
