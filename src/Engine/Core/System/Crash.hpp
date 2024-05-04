#pragma once
#include "Engine/Base.hpp"

namespace IzEngine
{
	/// <summary>
	/// Crash report class.
	/// </summary>
	class Crash
	{
	public:
		/// <summary>
		/// Setup handler.
		/// </summary>
		static void Setup();

		/// <summary>
		/// Exception handler.
		/// </summary>
		/// <param name="exception">The exception.</param>
		static void ExceptionHandler(void* exception);

		/// <summary>
		/// Creates a mini dump.
		/// </summary>
		/// <param name="exception">The exception.</param>
		static void MiniDump(void* exception);

		/// <summary>
		/// Create stack trace.
		/// </summary>
		/// <param name="exception">The exception.</param>
		static void StackTrace(void* exception);

	private:
		static inline UUID ID;
	};
}
