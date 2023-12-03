#include "Log.hpp"
#include "Game/Game.hpp"

namespace IW3SR
{
	Log::Log()
	{
		AllocConsole();
		SetConsoleTitle("IW3SR");

		freopen_s(reinterpret_cast<FILE**>(stdin), "CONIN$", "r", stdin);
		freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);
		freopen_s(reinterpret_cast<FILE**>(stderr), "CONOUT$", "w", stderr);
	}

	Log::~Log()
	{
		fclose(stdin);
		fclose(stdout);
		fclose(stderr);

		FreeConsole();
	}

	void Log::Write(int channel, int type, std::string msg)
	{
		Write(channel, msg.c_str(), type);
	}

	void Log::Write(int channel, const char* msg, int type)
	{
		std::cout << msg;
		if (Com_PrintMessage_h)
			Com_PrintMessage_h(channel, msg, type);
	}
}
