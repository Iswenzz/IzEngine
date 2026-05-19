#pragma once
#include "Engine/Core/Base/UUID.hpp"

namespace IzEngine
{
	class Crash
	{
	public:
		static void Setup();
		static void ExceptionHandler(void* exception);
		static void MiniDump(void* exception);
		static void StackTrace(void* exception);

	private:
		static inline UUID ID;
	};
}
