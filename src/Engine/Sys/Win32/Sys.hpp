#pragma once
#include "Engine/Sys/Win32.hpp"

namespace IW3SR::Engine
{
	/// <summary>
	/// Engine system.
	/// </summary>
	class Sys
	{
		CLASS_SINGLETON(Sys)
	public:
		HWND MainWindow = nullptr;

	private:
		/// <summary>
		/// Initialize system.
		/// </summary>
		Sys() = default;
		~Sys() = default;
	};
}
