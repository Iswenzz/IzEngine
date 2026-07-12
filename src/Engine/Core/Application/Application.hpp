#pragma once
#include "Engine/Core/Communication/Event.hpp"

namespace IzEngine
{
	class Application
	{
	public:
		static void Prepare();
		static void Initialize();
		static void Shutdown();
		static void Dispatch(Event& event);
	};
}
