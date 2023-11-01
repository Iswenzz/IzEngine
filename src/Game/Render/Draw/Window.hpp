#pragma once
#include "Utils/ImGUI.hpp"

namespace IW3SR
{
	/// <summary>
	/// Window drawing.
	/// </summary>
	class API Window
	{
	public:
		std::string Name;
		vec2 Position = vec2::Zero;
		vec2 Size = vec2::One;
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
		void SetRect(const vec2& position, const vec2& size);

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
		void Begin(ImGuiWindowFlags flags = 0);

		/// <summary>
		/// End drawing.
		/// </summary>
		void End();

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(Window, Name, Position, Size, Open);
	};
}
