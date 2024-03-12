#include "Text.hpp"
#include "Game/Renderer/Assets.hpp"

namespace IW3SR::Game
{
	Text::Text(const std::string& text, const std::string& font, float x, float y, float size, const vec4& color)
	{
		Value = text;
		Position = { x, y };
		Color = color;
		FontName = font;
	}

	void Text::SetRectAlignment(Horizontal horizontal, Vertical vertical)
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
		auto& assets = Assets::Get();
		Font = assets.LoadFont(font);
		FontName = font;
		FontIndex = std::distance(assets.FontNames.begin(), std::ranges::find(assets.FontNames, FontName));
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
		if (!ImGui::CollapsingHeader(label, open))
			return;

		ImGui::PushID(label.c_str());

		ImGui::DragFloat2("Position", Position);
		ImGui::ColorEdit4("Color", Color, ImGuiColorEditFlags_Float);

		if (ImGui::InputFloat("Font Size", &FontSize, 0.1))
			SetFont(FontName);

		const auto& fonts = Assets::Get().FontNames;
		if (ImGui::Combo("Font", &FontIndex, fonts))
			SetFont(fonts[FontIndex]);

		ImGui::ComboAlign(&AlignX, &AlignY);
		ImGui::ComboAlignRect(&HorizontalAlign, &VerticalAlign);

		ImGui::PopID();
	}

	void Text::Render()
	{
		float x = Position.x;
		float y = Position.y;
		float xScale = FontSize * FontRescale;
		float yScale = FontSize * FontRescale;

		if (!Font)
			SetFont(FontName);

		Size = { R_TextWidth(Value.c_str(), Value.size(), Font) * xScale, Font->pixelHeight * yScale };

		ComputeAlignment(x, y);
		UI::Get().Screen.Apply(x, y, xScale, yScale, HorizontalAlign, VerticalAlign);
		R_AddCmdDrawText(Value.c_str(), 0x7FFFFFFF, Font, x, y, xScale, yScale, 0, 0, Color);
	}
}
