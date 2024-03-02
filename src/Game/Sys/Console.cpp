#include "Console.hpp"

namespace IW3SR::Game
{
	Console::Console() : Con(Engine::Console::Get()) { }

	void Console::Initialize()
	{
		Con.Initialize();
		Con.SetTile("IW3SR");
	}

	void Console::Release()
	{
		Con.Release();
	}

	void Console::Write(ConChannel channel, const char* msg, int type)
	{
		Log::Write(Q3(msg));

		if (Com_PrintMessage_h)
			Com_PrintMessage_h(channel, msg, type);
	}

	void Console::ExecuteSingleCommand(int localClientNum, int controllerIndex, char* command)
	{
		if (std::string(command) == "openscriptmenu cj load")
			ModulesCallback(OnLoadPosition);

		Cmd_ExecuteSingleCommand_h(localClientNum, controllerIndex, command);
	}

	std::string Console::Q3(const std::string& msg)
	{
		std::string result;
		auto size = msg.size();

		for (int i = 0; i < size; i++)
		{
			if (msg[i] == '^' && i + 1 < size && msg[i + 1] != '^')
			{
				int color = ((msg[i + 1]) - '0') & 7;
				result += std::format("\x1b[{}m", static_cast<int>(Q3Colors[color]));
				i++;
				continue;
			}
			result += msg[i];
		}
		return result;
	}
}
