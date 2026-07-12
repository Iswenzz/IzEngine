#pragma once
#include "Engine/Core/Common.hpp"

namespace IzEngine
{
	class API Mouse
	{
	public:
		static inline vec2 Position;
		static inline vec2 Delta;
		static inline vec2 LastDelta;
		static inline int ScrollDelta = 0;

		static void Register();
		static void Process(uint64_t msg, uint64_t state);
	};
}
