#pragma once
#include "Core/Common.hpp"

#include "Renderer/Resources/Font.hpp"
#include "Renderer/Resources/Texture.hpp"

namespace IzEngine
{
	class API Draw2D
	{
	public:
		static void Text(const std::string& text, const Ref<Font>& font, const vec2& position, const vec2& size,
			const vec4& color);
		static void Text(const std::string& text, const Ref<Font>& font, const vec2& position, const vec2& size,
			const vec2& skew, const vec4& color);
		static vec2 TextSize(const std::string& text, const Ref<Font>& font);
		static void Rect(const Ref<Texture>& texture, const vec2& position, const vec2& size, const vec4& color);
	};
}
