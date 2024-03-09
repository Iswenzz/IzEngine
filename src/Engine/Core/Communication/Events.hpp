#pragma once
#include "Event.hpp"

namespace IW3SR::Engine
{
	class EventPluginRenderer : public Event
	{
		EVENT_CLASS("plugin.renderer")
		EventPluginRenderer() = default;
	};
}
