#pragma once
#include "Game/Render/GUI.hpp"

namespace IW3SR::UI
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
		void Frame();

		NLOHMANN_SERIALIZE_DERIVED_EMPTY(Modules, Window)
	};
}
