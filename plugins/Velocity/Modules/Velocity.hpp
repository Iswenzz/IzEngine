#pragma once
#include "Game/Plugin.hpp"

namespace IW3SR
{
	/// <summary>
	/// Draw current velocity.
	/// </summary>
	class Velocity : public Module
	{
	public:
		int Value;
		int Average;
		int Max;

		CircularBuffer<int, 1000> Values;
		CircularBuffer<int, 1000> Averages;
		CircularBuffer<int, 1000> Maxs;

		Text VelocityText;
		Text AverageText;
		Text MaxText;

		Window Graph;
		KeyListener ResetKey;
		bool ShowMax;
		bool ShowAverage;
		bool ShowGraph;

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

		NLOHMANN_SERIALIZE_POLY(Velocity, Module, VelocityText, MaxText, AverageText, Graph,
			ResetKey, ShowAverage, ShowMax, ShowGraph)
	};
}
