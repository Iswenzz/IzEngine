#pragma once
#include "Game/Plugin.hpp"

namespace IW3SR::Addons
{
	/// <summary>
	/// Draw lagometer.
	/// </summary>
	class Lagometer : public Module
	{
	public:
		vec4 SnapColor;
		vec4 SnapDelayColor;
		vec4 SnapFlagColor;
		vec4 SnapFlagDropColor;
		vec4 PingColor;

		CircularBuffer<int, 48> Snaps;
		CircularBuffer<int, 48> SnapsDelay;
		CircularBuffer<int, 48> SnapsFlags;
		CircularBuffer<int, 48> SnapsFlagsDrop;
		CircularBuffer<int, 48> Pings;
		Plots Graph;

		bool ShowSnap;
		bool ShowSnapFlag;
		bool ShowPing;

		/// <summary>
		/// Create the module.
		/// </summary>
		Lagometer();
		virtual ~Lagometer() = default;

		/// <summary>
		/// Menu drawing.
		/// </summary>
		void OnMenu() override;

		/// <summary>
		/// Render frame.
		/// </summary>
		void OnRender() override;

		NLOHMANN_SERIALIZE_POLY(Lagometer, Module, SnapColor, SnapDelayColor, SnapFlagColor, SnapFlagDropColor,
			PingColor, ShowSnap, ShowSnapFlag, ShowPing, Graph)
	};
}
