#pragma once
#include "Game/Definitions.hpp"
#include "Utils/Math.hpp"

namespace IW3SR
{
	/// <summary>
	/// HUD element.
	/// </summary>
	class HUD
	{
	public:
		vec2 Position = vec2::Zero;
		vec2 Size = vec2::One;
		vec4 Color = vec4::One;
		RectAlignHorizontal_t HorizontalAlign = HORIZONTAL_ALIGN_LEFT;
		RectAlignVertical_t VerticalAlign = VERTICAL_ALIGN_TOP;
		hudalign_t AlignX = HUDALIGN_LEFT;
		hudalign_t AlignY = HUDALIGN_TOP;

		/// <summary>
		/// Initialize the HUD.
		/// </summary>
		HUD() = default;
		virtual ~HUD() = default;

		/// <summary>
		/// Initialize the HUD.
		/// </summary>
		/// <param name="material">The material.</param>
		/// <param name="x">X position.</param>
		/// <param name="y">Y position.</param>
		/// <param name="w">The width.</param>
		/// <param name="h">The height.</param>
		/// <param name="color">The color.</param>
		HUD(const std::string& material, float x, float y, float w, float h, const vec4& color);

		/// <summary>
		/// Initialize the HUD.
		/// </summary>
		/// <param name="material">The material.</param>
		/// <param name="pos">XY position.</param>
		/// <param name="size">WH size.</param>
		/// <param name="color">The color.</param>
		HUD(const std::string& material, const vec2& pos, const vec2& size, const vec4& color);

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
		/// Compute the element alignment.
		/// </summary>
		/// <param name="x">X position.</param>
		/// <param name="y">Y position.</param>
		virtual void ComputeAlignment(float& x, float& y);

		/// <summary>
		/// Render HUD.
		/// </summary>
		virtual void Render();

	private:
		Material* Material;
		std::string MaterialName;
	};
}
