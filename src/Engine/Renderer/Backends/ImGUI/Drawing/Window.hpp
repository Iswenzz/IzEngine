#pragma once
#include "ImGUI/Components.hpp"

namespace IW3SR::Engine
{
	/// <summary>
	/// Window drawing.
	/// </summary>
	class API Window : public IObject
	{
	public:
		std::string Name;
		vec2 Position = { 20, 20 };
		vec2 Size = { 250, 140 };
		vec2 RenderPosition;
		vec2 RenderSize;
		bool Open = false;
		bool Designer = false;

		Horizontal HorizontalAlign = HORIZONTAL_LEFT;
		Vertical VerticalAlign = VERTICAL_TOP;

		/// <summary>
		/// Create a window.
		/// </summary>
		Window() = default;
		virtual ~Window() = default;

		/// <summary>
		/// Create a window.
		/// </summary>
		/// <param name="name">The window name.</param>
		Window(const std::string& name);

		/// <summary>
		/// Set designer edition.
		/// </summary>
		/// <param name="state">The state.</param>
		void SetDesigner(bool state);

		/// <summary>
		/// Set the window position and size.
		/// </summary>
		/// <param name="position">The position.</param>
		/// <param name="size">The size.</param>
		void SetRect(const vec2& position, const vec2& size);

		/// <summary>
		/// Set the rect alignment.
		/// </summary>
		/// <param name="horizontal">Horizontal aligment.</param>
		/// <param name="vertical">Vertical alignment.</param>
		void SetRectAlignment(Horizontal horizontal, Vertical vertical);

		/// <summary>
		/// Set the window position and size.
		/// </summary>
		/// <param name="x">X value.</param>
		/// <param name="y">Y value.</param>
		/// <param name="w">The width.</param>
		/// <param name="h">The height.</param>
		void SetRect(float x, float y, float w, float h);

		/// <summary>
		/// Begin drawing.
		/// </summary>
		/// <param name="flags">Window flags.</param>
		virtual void Begin(ImGuiWindowFlags flags = 0);

		/// <summary>
		/// Render menu.
		/// </summary>
		/// <param name="label">The label.</param>
		/// <param name="open">Default open.</param>
		virtual void Menu(const std::string& label, bool open = false);

		/// <summary>
		/// End drawing.
		/// </summary>
		virtual void End();

		SERIALIZE_POLY_BASE(Window, Position, Size, Open, HorizontalAlign, VerticalAlign)
	};
}
