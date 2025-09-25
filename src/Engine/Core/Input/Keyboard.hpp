#pragma once
#include "Core/Base.hpp"

namespace IzEngine
{
	/// <summary>
	/// Keyboard class.
	/// </summary>
	class API Keyboard
	{
	public:
		/// <summary>
		/// Register keyboard.
		/// </summary>
		/// <param name="window">The window handle.</param>
		static void Register(void* window);

		/// <summary>
		/// Process inputs.
		/// </summary>
		/// <param name="msg">The message code.</param>
		/// <param name="input">The input code.</param>
		static void Process(uint64_t msg, uint64_t input);
	};
}
