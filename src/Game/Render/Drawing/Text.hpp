#pragma once
#include "Game/Definitions.hpp"
#include "Math/Math.hpp"

namespace IW3SR
{
	/// <summary>
	/// Text element.
	/// </summary>
	class API Text
	{
	public:
		std::string Value;
		vec2 Position = vec2::Zero;
		vec4 Color = vec4::One;
		RectAlignHorizontal_t HorizontalAlign = HORIZONTAL_ALIGN_LEFT;
		RectAlignVertical_t VerticalAlign = VERTICAL_ALIGN_TOP;
		hudalign_t AlignX = HUDALIGN_LEFT;
		hudalign_t AlignY = HUDALIGN_TOP;

		ID3DXFont* Font;
		std::string FontName;
		float FontSize;
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
		/// Set the rect alignment.
		/// </summary>
		/// <param name="horizontal">Horizontal aligment.</param>
		/// <param name="vertical">Vertical alignment.</param>
		void SetRectAlignment(RectAlignHorizontal_t horizontal, RectAlignVertical_t vertical);

		/// <summary>
		/// Set the element alignment.
		/// </summary>
		/// <param name="horizontal">Horizontal aligment.</param>
		/// <param name="vertical">Vertical aligment.</param>
		void SetAlignment(hudalign_t horizontal, hudalign_t vertical);

		/// <summary>
		/// Set/update font.
		/// </summary>
		void SetFont(const std::string& font);

		/// <summary>
		/// Render text.
		/// </summary>
		virtual void Render();

	private:
		/// <summary>
		/// Compute the text alignment.
		/// </summary>
		/// <param name="x">X position.</param>
		/// <param name="y">Y position.</param>
		void ComputeAlignment(float& x, float& y);

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(Text, Value, Position, Color,
			HorizontalAlign, VerticalAlign, AlignX, AlignY, FontName, FontSize);
	};
}
