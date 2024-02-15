#pragma once
#include "Engine/Backends/ImGUI/Drawing/Window.hpp"

namespace IW3SR::Engine
{
	/// <summary>
	/// Plots drawing.
	/// </summary>
	class API Plots : public Window
	{
	public:
		/// <summary>
		/// Initialize the plots.
		/// </summary>
		Plots();
		virtual ~Plots() = default;

		/// <summary>
		/// Begin drawing.
		/// </summary>
		/// <param name="flags">Window flags.</param>
		void Begin(ImGuiWindowFlags flags = 0) override;
	};
}
