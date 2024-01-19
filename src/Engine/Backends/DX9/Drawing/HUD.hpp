#pragma once
#include "Engine/Backends/DX9.hpp"
#include "Engine/Backends/DX9/Resources/Texture.hpp"
#include "Engine/Core/Math.hpp"

namespace IW3SR
{
	/// <summary>
	/// HUD element.
	/// </summary>
	class API HUD : public IObject
	{
	public:
		std::string ID;
		vec2 Position = vec2::Zero;
		vec2 Size = vec2::One;
		vec2 RenderPosition = vec2::Zero;
		vec2 RenderSize = vec2::One;
		vec4 Color = vec4::One;
		RectAlignHorizontal HorizontalAlign = HORIZONTAL_ALIGN_LEFT;
		RectAlignVertical VerticalAlign = VERTICAL_ALIGN_TOP;
		Alignment AlignX = ALIGN_LEFT;
		Alignment AlignY = ALIGN_TOP;

		std::shared_ptr<Texture> Texture;
		std::string TextureID;

		/// <summary>
		/// Initialize the HUD.
		/// </summary>
		HUD() = default;
		virtual ~HUD() = default;

		/// <summary>
		/// Initialize the HUD.
		/// </summary>
		/// <param name="texture">The texture path.</param>
		/// <param name="x">X position.</param>
		/// <param name="y">Y position.</param>
		/// <param name="w">The width.</param>
		/// <param name="h">The height.</param>
		/// <param name="color">The color.</param>
		HUD(const std::string& texture, float x, float y, float w, float h, const vec4& color);

		/// <summary>
		/// Set the rect alignment.
		/// </summary>
		/// <param name="horizontal">Horizontal aligment.</param>
		/// <param name="vertical">Vertical alignment.</param>
		void SetRectAlignment(RectAlignHorizontal horizontal, RectAlignVertical vertical);

		/// <summary>
		/// Set the element alignment.
		/// </summary>
		/// <param name="horizontal">Horizontal aligment.</param>
		/// <param name="vertical">Vertical aligment.</param>
		void SetAlignment(Alignment horizontal, Alignment vertical);

		/// <summary>
		/// Set the texture.
		/// </summary>
		/// <param name="texture">The texture path.</param>
		void SetTexture(const std::string& texture);

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
		/// <summary>
		/// Compute the element alignment.
		/// </summary>
		/// <param name="x">X position.</param>
		/// <param name="y">Y position.</param>
		void ComputeAlignment(float& x, float& y);

		NLOHMANN_SERIALIZE_POLY_BASE(HUD, Position, Size, Color,
			HorizontalAlign, VerticalAlign, AlignX, AlignY, TextureID)
	};
}
