#pragma once
#include "Event.hpp"

namespace IzEngine
{
	class EventPluginGame : public Event
	{
		EVENT_CLASS("plugin.game")
		EventPluginGame() = default;
	};

	class EventConsoleCommand : public Event
	{
		EVENT_CLASS("console.command")
		EventConsoleCommand(std::string command) : command(command) { }
		std::string command;
	};
}
