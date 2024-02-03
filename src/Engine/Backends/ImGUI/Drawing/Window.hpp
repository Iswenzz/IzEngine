#pragma once
#include "Engine/Backends/ImGUI.hpp"
#include "Engine/Core/Math.hpp"

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
		vec2 Size = { 200, 140 };
		vec2 RenderPosition = vec2::Zero;
		vec2 RenderSize = vec2::Zero;
		bool Open = false;

		/// <summary>
		/// Initialize the window.
		/// </summary>
		Window() = default;
		virtual ~Window() = default;

		/// <summary>
		/// Initialize the window.
		/// </summary>
		/// <param name="name">The window name.</param>
		Window(std::string name);

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
		/// Begin drawing.
		/// </summary>
		virtual void Begin(ImGuiWindowFlags flags = 0);

		/// <summary>
		/// End drawing.
		/// </summary>
		virtual void End();

		NLOHMANN_SERIALIZE_POLY_BASE(Window, Name, Position, Size, Open)
	};
}
