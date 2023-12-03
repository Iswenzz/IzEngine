#pragma once
#include "Game/Render/GUI.hpp"

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

		NLOHMANN_DEFINE_DERIVED_TYPE_INTRUSIVE_EMPTY(Binds, Window)
	};
}
