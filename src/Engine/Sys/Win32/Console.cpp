#include "Console.hpp"

namespace IW3SR::Engine
{
	void Console::Initialize()
	{
		AllocConsole();

		freopen_s(reinterpret_cast<FILE**>(stdin), "CONIN$", "r", stdin);
		freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);
		freopen_s(reinterpret_cast<FILE**>(stderr), "CONOUT$", "w", stderr);
	}

	void Console::Release()
	{
		fclose(stdin);
		fclose(stdout);
		fclose(stderr);

		FreeConsole();
	}

	void Console::SetTile(const std::string& title)
	{
		SetConsoleTitle(title.c_str());
	}
}
