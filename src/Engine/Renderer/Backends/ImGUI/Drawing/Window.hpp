#pragma once
#include "ImGUI/Components.hpp"

namespace IzEngine
{
	/// <summary>
	/// Window drawing.
	/// </summary>
	class API Window : public IObject
	{
	public:
		std::string Name = UUID();
		vec2 Position = { 20, 20 };
		vec2 Size = { 250, 140 };
		vec2 RenderPosition;
		vec2 RenderSize;
		bool Open = false;
		bool Designer = false;

		ImGuiWindowFlags Flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
		Horizontal HorizontalAlign = HORIZONTAL_LEFT;
		Vertical VerticalAlign = VERTICAL_TOP;

		/// <summary>
		/// Create a window.
		/// </summary>
		Window() = default;
		virtual ~Window();

		/// <summary>
		/// Create a window.
		/// </summary>
		/// <param name="name">The window name.</param>
		Window(const std::string& name);

		/// <summary>
		/// Initialize window.
		/// </summary>
		virtual void Initialize();

		/// <summary>
		/// Release window.
		/// </summary>
		/// <returns></returns>
		virtual void Release();

		/// <summary>
		/// Set the window position and size.
		/// </summary>
		/// <param name="position">The position.</param>
		/// <param name="size">The size.</param>
		virtual void SetRect(const vec2& position, const vec2& size);

		/// <summary>
		/// Set the window position and size.
		/// </summary>
		/// <param name="x">X value.</param>
		/// <param name="y">Y value.</param>
		/// <param name="w">The width.</param>
		/// <param name="h">The height.</param>
		virtual void SetRect(float x, float y, float w, float h);

		/// <summary>
		/// Set the rect alignment.
		/// </summary>
		/// <param name="horizontal">Horizontal aligment.</param>
		/// <param name="vertical">Vertical alignment.</param>
		virtual void SetRectAlignment(Horizontal horizontal, Vertical vertical);

		/// <summary>
		/// Set designer edition.
		/// </summary>
		/// <param name="state">The state.</param>
		virtual void SetDesigner(bool state);

		/// <summary>
		/// Set window flags.
		/// </summary>
		/// <param name="flags">The window flags.</param>
		virtual void SetFlags(ImGuiWindowFlags flags);

		/// <summary>
		/// Render menu.
		/// </summary>
		/// <param name="label">The label.</param>
		/// <param name="open">Default open.</param>
		virtual void Menu(const std::string& label, bool open = false);

		/// <summary>
		/// Begin drawing.
		/// </summary>
		virtual void Begin();

		/// <summary>
		/// End drawing.
		/// </summary>
		virtual void End();

		/// <summary>
		/// Render frame.
		/// </summary>
		virtual void OnRender();

		/// <summary>
		/// On event.
		/// </summary>
		/// <param name="event">The event.</param>
		virtual void OnEvent(Event& event);

		SERIALIZE_POLY_BASE(Window, Position, Size, Open, HorizontalAlign, VerticalAlign)
	};
}
