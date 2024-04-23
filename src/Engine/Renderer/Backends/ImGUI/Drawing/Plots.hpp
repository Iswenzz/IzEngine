#pragma once
#include "ImGUI/Drawing/Widget.hpp"

namespace IzEngine
{
	/// <summary>
	/// Plots drawing.
	/// </summary>
	class API Plots : public Widget
	{
	public:
		/// <summary>
		/// Create plots.
		/// </summary>
		Plots();
		virtual ~Plots() = default;
	};
}
