#include "Draw2D.hpp"
#include "Engine/Backends/DX9/Device.hpp"

namespace IW3SR::Game
{
	void Draw2D::Rect(const std::string& material, float x, float y, float w, float h, const vec4& color)
	{
		Device::Get().Screen.Apply(x, y, w, h, HORIZONTAL_LEFT, VERTICAL_TOP);
		Material* handle = Material_RegisterHandle(material.c_str(), 3);
		R_AddCmdDrawStretchPic(handle, x, y, w, h, 0.f, 0.f, 0.f, 0.f, color);
	}

	void Draw2D::AngleYaw(const std::string& material, float start, float end, float yaw,
		float y, float h, const vec4& color)
	{
		const vec3 range = Math::AnglesToRange(start, end, yaw);
		if (!range.z)
		{
			Rect(material, range.x, y, range.y - range.x, h, color);
			return;
		}
		Rect(material, 0, y, range.x, h, color);
		Rect(material, range.y, y, SCREEN_WIDTH - range.y, h, color);
	}

	void Draw2D::LineYaw(const std::string& material, float angle, float yaw,
		float y, float w, float h, const vec4& color)
	{
		angle = Math::AngleNormalizePI(angle - yaw);

		if (!Math::AngleInFov(angle))
			return;

		const float x = Math::AngleScreenProjection(angle);
		Rect(material, x - w / 2, y, w, h, color);
	}

	void Draw2D::Text(const std::string& text, const std::string& font,
		float x, float y, float size, const vec4& color)
	{
		Font_s* f = Assets::Get().Fonts[font];
		Device::Get().Screen.Apply(x, y, HORIZONTAL_LEFT, VERTICAL_TOP);
		R_AddCmdDrawText(text.c_str(), 0x7FFFFFFF, f, x, y, size, size, 0, 0, color);
	}
}
