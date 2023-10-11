#include "Draw2D.hpp"

namespace IW3SR
{
	void Draw2D::Rect(std::string material, float x, float y, float w, float h, const vec4& color)
	{
		ScrPlace_ApplyRect(x, y, w, h, HORIZONTAL_ALIGN_LEFT, VERTICAL_ALIGN_TOP);
		Material* handle = Material_RegisterHandle(material.c_str(), 3);
		R_AddCmdDrawStretchPic(handle, x, y, w, h, 0.f, 0.f, 0.f, 0.f, color);
	}

	void Draw2D::AngleYaw(std::string material, float start, float end, float yaw, float y, float h, const vec4& color)
	{
		const range_t range = AnglesToRange(start, end, yaw);
		if (!range.split)
		{
			Rect(material, range.x1, y, range.x2 - range.x1, h, color);
			return;
		}
		Rect(material, 0, y, range.x1, h, color);
		Rect(material, range.x2, y, SCREEN_WIDTH - range.x2, h, color);
	}

	void Draw2D::LineYaw(std::string material, float angle, float yaw, float y, float w, float h, const vec4& color)
	{
		angle = AngleNormalizePi(angle - yaw);

		if (!AngleInFov(angle))
			return;

		const float x = AngleScreenProjection(angle);
		Rect(material, x - w / 2, y, w, h, color);
	}

	void Draw2D::Text(std::string text, Font_s* font, const vec2& pos, const vec2& size)
    {
        R_AddCmdDrawText(text.c_str(), 0x7FFFFFFF, font, pos.x, pos.y, size.x, size.y, 0, 0);
    }
}
