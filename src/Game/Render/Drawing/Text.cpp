#include "Text.hpp"
#include "Draw2D.hpp"

namespace IW3SR
{
	Text::Text(const std::string& text, const std::string& font, float x, float y, float size, const vec4& color)
	{
		ID = Utils::UUID();
		Value = text;
		Position = { x, y };
		Color = color;
		Font = nullptr;
		FontName = font;
		FontSize = size;
		FontIndex = 0;
	}

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
		int fontSize = std::floor(scr_place->scaleVirtualToReal[0] * FontSize * 14.f);
		Font = Assets::LoadFont(font, fontSize);
		FontName = font;
		
		if (!Font)
		{
			FontName = "Arial";
			Font = Assets::Fonts[FontName];
		}
		FontIndex = std::distance(Assets::FontNames.begin(), std::ranges::find(Assets::FontNames, FontName));
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
			HorizontalAlign = static_cast<RectAlignHorizontal_t>(horizontal + 1);

		int vertical = VerticalAlign - 1;
		if (ImGui::Combo("Vertical Alignment", &vertical, verticals))
			VerticalAlign = static_cast<RectAlignVertical_t>(vertical + 1);

		int alignX = AlignX / 4;
		if (ImGui::Combo("Align X", &alignX, horizontals))
			AlignX = static_cast<hudalign_t>(alignX * 4);

		int alignY = AlignY;
		if (ImGui::Combo("Align Y", &alignY, verticals))
			AlignY = static_cast<hudalign_t>(alignY);
	}

	void Text::Render()
	{
		float x = Position.x;
		float y = Position.y;

		if (!Font)
			SetFont(FontName);

		RECT textRect = { 0 };
		Font->DrawTextA(NULL, Value.c_str(), -1, &textRect, DT_CALCRECT, 0);
		RenderSize = { static_cast<float>(textRect.right - textRect.left), 
			static_cast<float>(textRect.bottom - textRect.top) };
		Size = vec2(scr_place->scaleRealToVirtual) * RenderSize;

		ComputeAlignment(x, y);
		Math::ApplyRect(x, y, HorizontalAlign, VerticalAlign);
		RECT rect = { static_cast<int>(x), static_cast<int>(y), 0, 0 };
		RenderPosition = { x, y };

		ImGui::Movable(ID, Position, Size, RenderPosition, RenderSize);
		Font->DrawTextA(NULL, Value.c_str(), -1, &rect, DT_NOCLIP, Color);
	}
}
