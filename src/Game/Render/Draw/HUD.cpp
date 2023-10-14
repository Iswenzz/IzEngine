#include "HUD.hpp"

namespace IW3SR
{
	HUD::HUD(const std::string& material, float x, float y, float w, float h, const vec4& color)
	{
		Material = nullptr;
		MaterialName = material;
		Position = { x, y };
		Size = { w, h };
		Color = color;
	}

	HUD::HUD(const std::string& material, const vec2& pos, const vec2& size, const vec4& color) :
		HUD(material, pos.x, pos.y, size.x, size.y, color) { }

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

	void HUD::ComputeAlignment(float& x, float& y)
	{
		if (AlignX & HUDALIGN_CENTER)
			x += -(Size.x / 2);
		else if (AlignX & HUDALIGN_RIGHT)
			x += Size.x;

		if (AlignY & HUDALIGN_MIDDLE)
			y += Size.y / 2;
		else if (AlignY & HUDALIGN_BOTTOM)
			y += Size.y;
	}

	void HUD::Render()
	{
		float x = Position.x;
		float y = Position.y;
		float w = Size.x;
		float h = Size.y;

		if (!Material)
			Material = Material_RegisterHandle(MaterialName.c_str(), 3);

		ComputeAlignment(x, y);
		ScrPlace_ApplyRect(x, y, w, h, HorizontalAlign, VerticalAlign);
		R_AddCmdDrawStretchPic(Material, x, y, w, h, 0.f, 0.f, 0.f, 0.f, Color);
	}
}
