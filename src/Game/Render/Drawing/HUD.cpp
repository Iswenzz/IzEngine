#include "HUD.hpp"
#include "Draw2D.hpp"

namespace IW3SR
{
	HUD::HUD(const std::string& texture, float x, float y, float w, float h, const vec4& color)
	{
		ID = Utils::UUID();
		Position = { x, y };
		Size = { w, h };
		Color = color;
		Texture = nullptr;
		TexturePath = texture;
	}

	void HUD::SetRectAlignment(RectAlignHorizontal_t horizontal, RectAlignVertical_t vertical)
	{
		HorizontalAlign = horizontal;
		VerticalAlign = vertical;
	}

	void HUD::SetAlignment(hudalign_t horizontal, hudalign_t vertical)
	{
		AlignX = horizontal;
		AlignY = vertical;
	}

	void HUD::SetTexture(const std::string& texture)
	{
		TexturePath = texture;
		Texture = Assets::Textures[texture];
		Texture->GetSurfaceLevel(0, &TextureSurface);
	}

	void HUD::ComputeAlignment(float& x, float& y)
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

	void HUD::Menu(const std::string& label, bool open)
	{
		if (!ImGui::CollapsingHeader(label.c_str(), open ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None))
			return;

		const std::vector<std::string>& horizontals = Draw2D::HorizontalAlignment;
		const std::vector<std::string>& verticals = Draw2D::VerticalAlignment;
		const std::vector<std::string>& fonts = Assets::FontNames;

		ImGui::DragFloat2("Position", Position);
		ImGui::DragFloat2("Size", Size);
		ImGui::ColorEdit4("Color", Color, ImGuiColorEditFlags_Float);

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

	void HUD::Render()
	{
		float x = Position.x;
		float y = Position.y;
		float w = Size.x;
		float h = Size.y;

		if (!Texture)
			SetTexture(TexturePath);

		ComputeAlignment(x, y);
		Math::ApplyRect(x, y, w, h, HorizontalAlign, VerticalAlign);
		RECT rect = { static_cast<int>(x), static_cast<int>(y), static_cast<int>(w), static_cast<int>(h) };
		RenderPosition = { x, y };
		RenderSize = { w, h };

		ImGui::Movable(ID, Position, Size, RenderPosition, RenderSize);
		dx->device->StretchRect(TextureSurface.Get(), NULL, NULL, &rect, D3DTEXF_NONE);
	}
}
