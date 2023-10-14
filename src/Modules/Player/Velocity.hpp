#pragma once
#include "Modules/Module.hpp"
#include "Game/Render/Draw/Text.hpp"

namespace IW3SR
{
	/// <summary>
	/// Draw current velocity.
	/// </summary>
	class Velocity : public Module
	{
	public:
		Text VelocityText;
		vec4 Color;

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
