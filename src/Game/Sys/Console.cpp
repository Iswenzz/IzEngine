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

	void Console::Write(int channel, const char* msg, int type)
	{
		Log::Write(channel, msg);

		if (Com_PrintMessage_h)
			Com_PrintMessage_h(channel, msg, type);
	}
}
