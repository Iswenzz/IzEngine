#pragma once
#include "Engine/Core/Communication/Event.hpp"

namespace IzEngine
{
	class Application
	{
	public:
		static void Start();
		static void Shutdown();
		static void Dispatch(Event& event);
	};
}
