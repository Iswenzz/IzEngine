#include "Text.hpp"
#include "Draw2D.hpp"

#include "Engine/Backend/ImGUI/Common.hpp"
#include "Engine/Core/System/AssetManager.hpp"

namespace IzEngine
{
	Text::Text(const std::string& text, const std::string& font, float x, float y, float size, const vec4& color)
	{
		Value = text;
		Position = { x, y };
		Color = color;
		FontName = font;
		FontSize = size;
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
		int fontSize = floor(UI::Size * FontSize * FontRescale);

		Font = Font::Create({ .ID = font, .Height = fontSize });
		FontName = font;
		FontIndex =
			std::distance(AssetManager::FontNames.begin(), std::ranges::find(AssetManager::FontNames, FontName));
	}

	void Text::ComputeAlignment(vec2& position)
	{
		if (AlignX == Alignment::Center)
			position.x += -(Size.x / 2.f);
		else if (AlignX == Alignment::Right)
			position.x += -Size.x;

		if (AlignY == Alignment::Middle)
			position.y += Size.y / 2.f;
		else if (AlignY == Alignment::Bottom)
			position.y += Size.y;
	}

	void Text::Menu(const std::string& label, bool open)
	{
		if (!ImGui::BeginSection(label, open))
			return;

		ImGui::Property("Position");
		ImGui::DragFloat2("##position", &Position.x);
		ImGui::Property("Skew");
		ImGui::DragFloat2("##skew", &Skew.x, 0.01, -0.5, 0.5);
		ImGui::Property("Color");
		ImGui::ColorEdit4("##color", &Color.x, ImGuiColorEditFlags_Float);

		ImGui::Property("Font Size");
		if (ImGui::InputFloat("##fontsize", &FontSize, 0.1))
			SetFont(FontName);

		const auto& fonts = AssetManager::FontNames;
		ImGui::Property("Font");
		if (ImGui::Combo("##font", &FontIndex, fonts))
			SetFont(fonts[FontIndex]);

		ImGui::ComboAlign(&AlignX, &AlignY);
		ImGui::ComboAlignRect(&HorizontalAlign, &VerticalAlign);

		ImGui::EndSection();
	}

	void Text::Render()
	{
		if (!Font)
			SetFont(FontName);

		RenderSize = Draw2D::GetTextSize(Value, Font);
		Size = UI::Screen.RealToVirtual * RenderSize;

		vec2 position = Position;
		vec2 size = Size;

		ComputeAlignment(position);
		UI::Screen.Apply(position, HorizontalAlign, VerticalAlign);
		RenderPosition = position;

		ImGui::Movable(ID, Position, Size, RenderPosition, RenderSize);
		Draw2D::DrawText(Value, Font, vec3(RenderPosition, 0), RenderSize.x, Color, Skew);
	}
}
