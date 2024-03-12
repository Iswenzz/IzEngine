#pragma once
#include "Game/Definitions.hpp"

namespace IW3SR::Game
{
	/// <summary>
	/// HUD element.
	/// </summary>
	class API HUD : public IObject
	{
	public:
		vec2 Position;
		vec2 Size;
		vec4 Color = vec4::One;

		Horizontal HorizontalAlign = HORIZONTAL_LEFT;
		Vertical VerticalAlign = VERTICAL_TOP;
		Alignment AlignX = ALIGN_LEFT;
		Alignment AlignY = ALIGN_TOP;

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
		void SetRectAlignment(Horizontal horizontal, Vertical vertical);

		/// <summary>
		/// Set the element alignment.
		/// </summary>
		/// <param name="horizontal">Horizontal aligment.</param>
		/// <param name="vertical">Vertical aligment.</param>
		void SetAlignment(Alignment horizontal, Alignment vertical);

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
		void Menu(const std::string& label, bool open = false);

		/// <summary>
		/// Render HUD.
		/// </summary>
		void Render();

	private:
		Material* Material = nullptr;

		/// <summary>
		/// Compute the element alignment.
		/// </summary>
		/// <param name="x">X position.</param>
		/// <param name="y">Y position.</param>
		void ComputeAlignment(float& x, float& y);

		SERIALIZE_POLY_BASE(HUD, Position, Size, Color,
			HorizontalAlign, VerticalAlign, AlignX, AlignY, MaterialName)
	};
}
