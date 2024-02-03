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
		vec2 Position = { 20, 380 };
		vec2 Size = { 160, 100 };

		/// <summary>
		/// Initialize the plots.
		/// </summary>
		Plots();
		virtual ~Plots() = default;

		/// <summary>
		/// Begin drawing.
		/// </summary>
		void Begin(ImGuiWindowFlags flags = 0) override;

		NLOHMANN_SERIALIZE_POLY(Plots, Window, Position, Size)
	};
}
