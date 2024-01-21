#include "Console.hpp"

namespace IW3SR::Game
{
	Console::Console()
	{
		AllocConsole();
		SetConsoleTitle("IW3SR");

		freopen_s(reinterpret_cast<FILE**>(stdin), "CONIN$", "r", stdin);
		freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);
		freopen_s(reinterpret_cast<FILE**>(stderr), "CONOUT$", "w", stderr);
	}

	Console::~Console()
	{
		fclose(stdin);
		fclose(stdout);
		fclose(stderr);

		FreeConsole();
	}

	void Console::Write(int channel, const char* msg, int type)
	{
		Log::Write(channel, msg);

		if (Com_PrintMessage_h)
			Com_PrintMessage_h(channel, msg, type);
	}
}
