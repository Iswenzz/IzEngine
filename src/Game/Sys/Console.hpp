#pragma once
#include "Game/Definitions.hpp"
#include "Sys/Win32.hpp"

namespace IW3SR
{
	/// <summary>
	/// Console class.
	/// </summary>
	class Console
	{
	public:
		/// <summary>
		/// Initialize the console.
		/// </summary>
		Console();

		/// <summary>
		/// Release the console.
		/// </summary>
		~Console();

		/// <summary>
		/// Write to the console.
		/// </summary>
		/// <param name="channel">The channel.</param>
		/// <param name="msg">The message.</param>
		/// <param name="type">The messsage type.</param>
		static void Write(int channel, const char* msg, int type);
	};
}
