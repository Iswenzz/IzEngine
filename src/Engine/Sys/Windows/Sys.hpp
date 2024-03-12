#pragma once
#include "Windows/Base.hpp"

namespace IW3SR::Engine
{
	/// <summary>
	/// Engine system.
	/// </summary>
	class Sys
	{
	public:
		static inline HWND MainWindow = nullptr;

		/// <summary>
		/// Is debug mode.
		/// </summary>
		/// <returns></returns>
		static bool IsDebug();
	};
}
