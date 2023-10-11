#pragma once

#include "Modules/Module.hpp"
#include "Game/Render/Draw.hpp"

namespace IW3SR
{
	/// <summary>
	/// Draw current Velocity using engine renderer.
	/// </summary>
	class Velocity : public Module
	{
	public:
		vec2 Pos;
		vec2 Size;

		/// <summary>
		/// Initialize the module.
		/// </summary>
		Velocity();
		virtual ~Velocity() = default;

		/// <summary>
		/// Draw 2D.
		/// </summary>
		void OnDraw2D() override;
	};
}