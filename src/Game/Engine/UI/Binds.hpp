#pragma once
#include "Game/Engine/GUI.hpp"

namespace IW3SR::UI
{
	/// <summary>
	/// Binds window.
	/// </summary>
	class Binds : public Window
	{
	public:
		/// <summary>
		/// Initialize the binds window.
		/// </summary>
		Binds();
		virtual ~Binds() = default;

		/// <summary>
		/// Render frame.
		/// </summary>
		void Frame();

		NLOHMANN_SERIALIZE_DERIVED_EMPTY(Binds, Window)
	};
}
