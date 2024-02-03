#pragma once
#include "Engine/Backends/ImGUI/Drawing/Window.hpp"

namespace IW3SR::Engine::UC
{
	/// <summary>
	/// Game modules.
	/// </summary>
	class Modules : public Window
	{
	public:
		/// <summary>
		/// Initialize the modules.
		/// </summary>
		Modules();
		virtual ~Modules() = default;

		/// <summary>
		/// Render frame.
		/// </summary>
		void Render();
	};
}
