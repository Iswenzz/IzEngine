#pragma once
#include "Engine/Backends/DX9.hpp"
#include "Engine/Backends/DX9/Resources/Font.hpp"
#include "Engine/Core/Math.hpp"

namespace IW3SR::Engine
{
	/// <summary>
	/// Text element.
	/// </summary>
	class API Text : public IObject
	{
	public:
		std::string Value;
		vec2 Position;
		vec2 Size;
		vec2 Scale = vec2::One;
		vec2 RenderPosition;
		vec2 RenderSize;
		vec4 Color = vec4::One;

		Horizontal HorizontalAlign = HORIZONTAL_LEFT;
		Vertical VerticalAlign = VERTICAL_TOP;
		Alignment AlignX = ALIGN_LEFT;
		Alignment AlignY = ALIGN_TOP;

		std::string FontName;
		float FontRescale = 13.33f;
		float FontSize = 1.4;
		int FontIndex = 0;

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
		/// Set the rect alignment.
		/// </summary>
		/// <param name="horizontal">Horizontal aligment.</param>
		/// <param name="vertical">Vertical alignment.</param>
		void SetRectAlignment(Horizontal horizontal, Vertical vertical);

		/// <summary>
		/// Set the element alignment.
		/// </summary>
		/// <param name="horizontal">Horizontal aligment.</param>
		/// <param name="vertical">Vertical aligment.</param>
		void SetAlignment(Alignment horizontal, Alignment vertical);

		/// <summary>
		/// Set/update font.
		/// </summary>
		void SetFont(const std::string& font);

		/// <summary>
		/// Render menu.
		/// </summary>
		/// <param name="label">The label.</param>
		/// <param name="open">Default open.</param>
		void Menu(const std::string& label, bool open = false);

		/// <summary>
		/// Render text.
		/// </summary>
		void Render();

	private:
		std::shared_ptr<Font> Font = nullptr;

		/// <summary>
		/// Compute the text alignment.
		/// </summary>
		/// <param name="x">X position.</param>
		/// <param name="y">Y position.</param>
		void ComputeAlignment(float& x, float& y);

		NLOHMANN_SERIALIZE_POLY_BASE(Text, Value, Position, Color,
			HorizontalAlign, VerticalAlign, AlignX, AlignY, FontName, FontSize)
	};
}
