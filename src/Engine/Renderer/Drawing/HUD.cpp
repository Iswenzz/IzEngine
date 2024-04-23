#include "HUD.hpp"
#include "Draw2D.hpp"

#include "ImGUI/UI.hpp"

namespace IzEngine
{
	HUD::HUD(const std::string& texture, float x, float y, float w, float h, const vec4& color)
	{
		Position = { x, y };
		Size = { w, h };
		Color = color;
		TextureName = texture;
	}

	void HUD::SetRectAlignment(Horizontal horizontal, Vertical vertical)
	{
		HorizontalAlign = horizontal;
		VerticalAlign = vertical;
	}

	void HUD::SetAlignment(Alignment horizontal, Alignment vertical)
	{
		AlignX = horizontal;
		AlignY = vertical;
	}

	void HUD::SetTexture(const std::string& texture)
	{
		Texture = Textures::List[texture];
		TextureName = texture;
	}

	void HUD::ComputeAlignment(vec2& position)
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

	void HUD::Menu(const std::string& label, bool open)
	{
		if (!ImGui::CollapsingHeader(label, open))
			return;

		ImGui::PushID(label.c_str());

		ImGui::DragFloat2("Position", Position);
		ImGui::DragFloat2("Size", Size);
		ImGui::ColorEdit4("Color", Color, ImGuiColorEditFlags_Float);

		ImGui::ComboAlign(&AlignX, &AlignY);
		ImGui::ComboAlignRect(&HorizontalAlign, &VerticalAlign);

		ImGui::PopID();
	}

	void HUD::Render()
	{
		if (!Texture)
			SetTexture(TextureName);

		vec2 position = Position;
		vec2 size = Size;

		ComputeAlignment(position);
		UI::Get().Screen.Apply(position, size, HorizontalAlign, VerticalAlign);
		RenderPosition = position;
		RenderSize = size;

		Draw2D::Rect(Texture, RenderPosition, RenderSize);
	}
}
