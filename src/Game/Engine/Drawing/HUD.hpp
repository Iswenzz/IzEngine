#pragma once
#include "Game/Definitions.hpp"
#include "Math/Math.hpp"

namespace IW3SR::Engine
{
	/// <summary>
	/// HUD element.
	/// </summary>
	class API HUD
	{
	public:
		vec2 Position = vec2::Zero;
		vec2 Size = vec2::One;
		vec4 Color = vec4::One;
		RectAlignHorizontal_t HorizontalAlign = HORIZONTAL_ALIGN_LEFT;
		RectAlignVertical_t VerticalAlign = VERTICAL_ALIGN_TOP;
		hudalign_t AlignX = HUDALIGN_LEFT;
		hudalign_t AlignY = HUDALIGN_TOP;

		Material* Material;
		std::string MaterialName;

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
		/// Set the material.
		/// </summary>
		/// <param name="material">The material name.</param>
		void SetMaterial(const std::string& material);

		/// <summary>
		/// Render menu.
		/// </summary>
		/// <param name="label">The label.</param>
		/// <param name="open">Default open.</param>
		virtual void Menu(const std::string& label, bool open = false);

		/// <summary>
		/// Render HUD.
		/// </summary>
		virtual void Render();

	private:
		/// <summary>
		/// Compute the element alignment.
		/// </summary>
		/// <param name="x">X position.</param>
		/// <param name="y">Y position.</param>
		virtual void ComputeAlignment(float& x, float& y);

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(HUD, Position, Size, Color, 
			HorizontalAlign, VerticalAlign, AlignX, AlignY, MaterialName)
	};
}
