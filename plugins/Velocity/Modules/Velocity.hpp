#pragma once
#include "Modules/Module.hpp"

namespace IW3SR
{
	/// <summary>
	/// Draw current velocity.
	/// </summary>
	class Velocity : public Module
	{
	public:
		Engine::Text VelocityText;
		vec4 Color;

		/// <summary>
		/// Initialize the module.
		/// </summary>
		Velocity();
		virtual ~Velocity() = default;

		/// <summary>
		/// Menu drawing.
		/// </summary>
		void OnMenu() override;

		/// <summary>
		/// Draw 2D.
		/// </summary>
		void OnDraw2D() override;

		NLOHMANN_DEFINE_POLY(Velocity, Module, VelocityText, Color);
	};
}
