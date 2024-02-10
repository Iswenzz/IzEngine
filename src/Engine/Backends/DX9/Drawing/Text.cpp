#include "Text.hpp"
#include "Draw2D.hpp"

#include "Engine/Backends/DX9/Device.hpp"
#include "Engine/Backends/DX9/Assets.hpp"
#include "Engine/Backends/ImGUI/UI.hpp"

namespace IW3SR::Engine
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
		int fontSize = std::floor(UI::Get().Size * FontSize * FontRescale);
		Font = assets.LoadFont(font, fontSize);
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
		if (!ImGui::CollapsingHeader(label.c_str(), open ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None))
			return;

		ImGui::PushID(ID.c_str());

		const std::vector<std::string>& horizontals = Draw2D::HorizontalAlignment;
		const std::vector<std::string>& verticals = Draw2D::VerticalAlignment;
		const std::vector<std::string>& fonts = Assets::Get().FontNames;

		ImGui::DragFloat2("Position", Position);
		ImGui::ColorEdit4("Color", Color, ImGuiColorEditFlags_Float);

		if (ImGui::InputFloat("Font Size", &FontSize, 0.1))
			SetFont(FontName);
		if (ImGui::Combo("Font", &FontIndex, fonts))
			SetFont(fonts[FontIndex]);

		int horizontal = HorizontalAlign - 1;
		if (ImGui::Combo("Horizontal Alignment", &horizontal, horizontals))
			HorizontalAlign = static_cast<Horizontal>(horizontal + 1);

		int vertical = VerticalAlign - 1;
		if (ImGui::Combo("Vertical Alignment", &vertical, verticals))
			VerticalAlign = static_cast<Vertical>(vertical + 1);

		int alignX = AlignX / 4;
		if (ImGui::Combo("Align X", &alignX, horizontals))
			AlignX = static_cast<Alignment>(alignX * 4);

		int alignY = AlignY;
		if (ImGui::Combo("Align Y", &alignY, verticals))
			AlignY = static_cast<Alignment>(alignY);

		ImGui::PopID();
	}

	void Text::Render()
	{
		float x = Position.x;
		float y = Position.y;

		if (!Font)
			SetFont(FontName);

		RECT textRect = { 0 };
		Font->Base->DrawTextA(NULL, Value.c_str(), -1, &textRect, DT_CALCRECT, 0);
		RenderSize = { static_cast<float>(textRect.right - textRect.left),
			static_cast<float>(textRect.bottom - textRect.top) };
		Size = UI::Get().Screen.RealToVirtual * RenderSize;

		ComputeAlignment(x, y);
		UI::Get().Screen.Apply(x, y, HorizontalAlign, VerticalAlign);
		RECT rect = { static_cast<int>(x), static_cast<int>(y), 0, 0 };
		RenderPosition = { x, y };

		ImGui::Movable(ID, Position, Size, RenderPosition, RenderSize);
		Font->Base->DrawTextA(NULL, Value.c_str(), -1, &rect, DT_NOCLIP, Color);
	}
}
