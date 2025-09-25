#pragma once
#include "Core/Base.hpp"

namespace IzEngine
{
	/// <summary>
	/// Mouse class.
	/// </summary>
	class API Mouse
	{
	public:
		static inline int X = 0;
		static inline int Y = 0;
		static inline int DeltaX = 0;
		static inline int DeltaY = 0;

		/// <summary>
		/// Register mouse.
		/// </summary>
		/// <param name="window">The window handle.</param>
		static void Register(void* window);

		/// <summary>
		/// Process raw input.
		/// </summary>
		/// <param name="msg">The message code.</param>
		/// <param name="state">The mouse state.</param>
		static void Process(uint64_t msg, uint64_t state);
	};
}
