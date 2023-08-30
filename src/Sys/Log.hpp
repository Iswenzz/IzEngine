#pragma once
#include "Game/Definitions.hpp"
#include "Sys/Hook.hpp"

namespace IW3SR
{
	/// <summary>
	/// Log class.
	/// </summary>
	class Log
	{
	public:
		static inline Hook<Com_PrintMessage_t> Com_PrintMessage_h;

		/// <summary>
		/// Initialize a new logger.
		/// </summary>
		Log();
		~Log() = default;
	};
}
