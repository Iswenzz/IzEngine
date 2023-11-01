#pragma once
#include "HUD.hpp"

namespace IW3SR
{
	/// <summary>
	/// Text element.
	/// </summary>
	class API Text : public HUD
	{
	public:
		std::string Value;

		Font_s* Font;
		std::string FontName;
		int FontIndex;

		/// <summary>
		/// Initialize the Text.
		/// </summary>
		Text() = default;
		virtual ~Text() = default;

		/// <summary>
		/// Initialize the Text.
		/// </summary>
		/// <param name="text">The text.</param>
		/// <param name="font">The font.</param>
		/// <param name="x">X value.</param>
		/// <param name="y">Y value.</param>
		/// <param name="size">Font size.</param>
		/// <param name="color">The color.</param>
		Text(const std::string& text, const std::string& font, float x, float y, float size, const vec4& color);

		/// <summary>
		/// Initialize the Text.
		/// </summary>
		/// <param name="text">The text.</param>
		/// <param name="font">The font.</param>
		/// <param name="pos">XY-coordinates of screen.</param>
		/// <param name="size">Font size.</param>
		/// <param name="color">The color.</param>
		Text(const std::string& text, const std::string& font, const vec2& pos, float size, const vec4& color);

		/// <summary>
		/// Set/update font.
		/// </summary>
		void SetFont(const std::string& font);

		/// <summary>
		/// Render text.
		/// </summary>
		void Render() override;

	private:
		/// <summary>
		/// Compute the text alignment.
		/// </summary>
		/// <param name="x">X position.</param>
		/// <param name="y">Y position.</param>
		void ComputeAlignment(float& x, float& y);

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(Text, Value, FontName, FontIndex);
	};
}
