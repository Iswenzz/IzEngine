#include "Console.hpp"

namespace IW3SR::Game
{
	Console::Console() : Con(Engine::Console::Get()) { }

	void Console::Initialize()
	{
		Con.Initialize();
		Con.SetTile("IW3SR");

		SetConsoleCtrlHandler(ConsoleHandler, true);
	}

	void Console::Release()
	{
		Con.Release();
	}

	void Console::Write(int channel, const char* msg, int type)
	{
		Log::Write(channel, msg);

		if (Com_PrintMessage_h)
			Com_PrintMessage_h(channel, msg, type);
	}

	BOOL Console::ConsoleHandler(DWORD dwCtrlType)
	{
		switch (dwCtrlType)
		{
		case CTRL_C_EVENT:
		case CTRL_CLOSE_EVENT:
			Cmd_ExecuteSingleCommand(0, 0, "quit");
			return true;
		}
		return false;
	}
}
