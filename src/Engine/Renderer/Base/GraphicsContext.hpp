#pragma once
#include "Engine/Core/Common.hpp"

namespace IzEngine
{
	class GraphicsContext
	{
	public:
		virtual ~GraphicsContext() = default;

		virtual void Initialize() = 0;
		virtual void Shutdown() = 0;

		static Scope<GraphicsContext> Create(void* window);
	};
}
