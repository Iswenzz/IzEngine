#pragma once
#include "Engine/Core/Base/UUID.hpp"

namespace IzEngine
{
	class Crash
	{
	public:
		static void Setup();
		static long STDCALL ExceptionHandler(void* exception);
		static void MiniDump(void* exception);
		static void StackTrace(void* exception);
		static void Patch(uintptr_t base);

	private:
		static inline UUID ID;
		static inline std::atomic_flag Handling;
	};
}
