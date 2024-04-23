#include "Text.hpp"
#include "Draw2D.hpp"

#include "ImGUI/UI.hpp"

namespace IzEngine
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
		int fontSize = floor(UI::Get().Size * FontSize * FontRescale);

		Font = Font::Create(font, fontSize);
		FontName = font;
		FontIndex = std::distance(Fonts::Names.begin(), std::ranges::find(Fonts::Names, FontName));
	}

	void Text::ComputeAlignment(vec2& position)
	{
		if (AlignX & ALIGN_CENTER)
			position.x += -(Size.x / 2.f);
		else if (AlignX & ALIGN_RIGHT)
			position.x += -Size.x;

		if (AlignY & ALIGN_MIDDLE)
			position.y += Size.y / 2.f;
		else if (AlignY & ALIGN_BOTTOM)
			position.y += Size.y;
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

		const auto& fonts = Fonts::Names;
		if (ImGui::Combo("Font", &FontIndex, fonts))
			SetFont(fonts[FontIndex]);

		ImGui::ComboAlign(&AlignX, &AlignY);
		ImGui::ComboAlignRect(&HorizontalAlign, &VerticalAlign);

		ImGui::PopID();
	}

	void Text::Render()
	{
		if (!Font)
			SetFont(FontName);

		RenderSize = Draw2D::TextSize(Value, Font);
		Size = UI::Get().Screen.RealToVirtual * RenderSize;

		vec2 position = Position;
		vec2 size = Size;

		ComputeAlignment(position);
		UI::Get().Screen.Apply(position, HorizontalAlign, VerticalAlign);
		RenderPosition = position;

		ImGui::Movable(ID, Position, Size, RenderPosition, RenderSize);
		Draw2D::Text(Value, Font, RenderPosition, RenderSize, Color);
	}
}
