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
		int Value;
		int Max;
		std::vector<int> Values;

		Text VelocityText;
		Text MaxVelocityText;
		KeyListener ResetKey;
		bool ShowMaxVelocity;
		bool ShowPlot;

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

		NLOHMANN_SERIALIZE_POLY(Velocity, Module, VelocityText, MaxVelocityText, ResetKey, ShowMaxVelocity, ShowPlot)
	};
}
