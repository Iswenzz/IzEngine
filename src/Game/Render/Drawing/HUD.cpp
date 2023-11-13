#include "HUD.hpp"
#include "Draw2D.hpp"

namespace IW3SR
{
	HUD::HUD(const std::string& texture, float x, float y, float w, float h, const vec4& color)
	{
		Texture = nullptr;
		TexturePath = texture;
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
		dx->device->StretchRect(TextureSurface, NULL, NULL, &rect, D3DTEXF_NONE);
	}
}
