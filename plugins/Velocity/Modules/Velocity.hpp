#pragma once
#include "Game/Modules.hpp"

namespace IW3SR
{
	/// <summary>
	/// Draw current velocity.
	/// </summary>
	class Velocity : public Module
	{
	public:
		Text VelocityText;

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
		/// Render frame.
		/// </summary>
		void OnFrame() override;

		NLOHMANN_SERIALIZE_POLY(Velocity, Module, VelocityText)
	};
}
