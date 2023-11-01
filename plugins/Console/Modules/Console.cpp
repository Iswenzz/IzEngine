#include "Console.hpp"

#include <Windows.h>
#include <iostream>

namespace IW3SR
{
    Console::Console() : Module("sr.sys.console", "Console") { }

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
