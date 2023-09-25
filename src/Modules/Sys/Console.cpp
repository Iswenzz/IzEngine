#include "Console.hpp"

#include <Windows.h>
#include <iostream>

namespace IW3SR
{
    Console::Console()
    {
        Sleep(5000);

        ID = "sr.sys.console";
        Name = "External Console";

        Initialize();
    }

    Console::~Console()
    {
        Shutdown();
    }

    void Console::Initialize()
    {
        AllocConsole();

        freopen_s(reinterpret_cast<FILE**>(stdin), "CONIN$", "r", stdin);
        freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);
        freopen_s(reinterpret_cast<FILE**>(stderr), "CONOUT$", "w", stderr);
    }

    void Console::Shutdown()
    {
        FreeConsole();
    }
}
