#pragma once
#include "Core/Math.hpp"

#include "Renderer/Resources/Font.hpp"
#include "Renderer/Resources/Texture.hpp"

namespace IzEngine
{
	/// <summary>
	/// Draw 2D class.
	/// </summary>
	class API Draw2D
	{
	public:
		/// <summary>
		/// Draw text.
		/// </summary>
		/// <param name="text">The text.</param>
		/// <param name="font">The font.</param>
		/// <param name="position">The position.</param>
		/// <param name="size">The size.</param>
		/// <param name="color">The color.</param>
		static void Text(const std::string& text, const Ref<Font>& font, const vec2& position, const vec2& size,
			const vec4& color);

		/// <summary>
		/// Get the text size.
		/// </summary>
		/// <param name="text">The text.</param>
		/// <param name="font">The font.</param>
		/// <returns></returns>
		static vec2 TextSize(const std::string& text, const Ref<Font>& font);

		/// <summary>
		/// Draw rect.
		/// </summary>
		/// <param name="texture">The texture.</param>
		/// <param name="position">The position.</param>
		/// <param name="size">The size.</param>
		static void Rect(const Ref<Texture>& texture, const vec2& position, const vec2& size);
	};
}
