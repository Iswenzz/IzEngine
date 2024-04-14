#pragma once
#include "Event.hpp"

namespace IW3SR::Engine
{
	class EventPluginRenderer : public Event
	{
		EVENT_CLASS("plugin.renderer")
		EventPluginRenderer() = default;
	};

	class EventConsoleCommand : public Event
	{
		EVENT_CLASS("console.command")
		EventConsoleCommand(std::string command) : command(command) { }
		std::string command;
	};
}
