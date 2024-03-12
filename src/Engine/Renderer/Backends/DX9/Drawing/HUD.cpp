#include "HUD.hpp"

#include "ImGUI/UI.hpp"
#include "DX9/Device.hpp"
#include "DX9/Assets.hpp"

namespace IW3SR::Engine
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
		Texture = Assets::Get().Textures[texture];
		TextureName = texture;
	}

	void HUD::ComputeAlignment(float& x, float& y)
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
		float x = Position.x;
		float y = Position.y;
		float w = Size.x;
		float h = Size.y;

		if (!Texture)
			SetTexture(TextureName);

		ComputeAlignment(x, y);
		UI::Get().Screen.Apply(x, y, w, h, HorizontalAlign, VerticalAlign);
		RECT rect = { static_cast<int>(x), static_cast<int>(y), static_cast<int>(w), static_cast<int>(h) };
		RenderPosition = { x, y };
		RenderSize = { w, h };

		ImGui::Movable(ID, Position, Size, RenderPosition, RenderSize);
		Device::Get().D3Device->StretchRect(Texture->BaseSurface, NULL, NULL, &rect, D3DTEXF_NONE);
	}
}
