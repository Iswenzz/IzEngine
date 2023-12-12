#include "Game/Game.hpp"

void Init()
{
    SR = new Game();
    SR->Initialize();
}

void Shutdown()
{
    delete SR;
}

ENTRY BOOL STDCALL RIB_Main(HANDLE handle, INT upDown)
{
    return TRUE;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved) 
{
    switch (dwReason) 
    {
        case DLL_PROCESS_ATTACH:
            Init();
            break;
        case DLL_THREAD_ATTACH:
            break;
        case DLL_THREAD_DETACH:
            break;
        case DLL_PROCESS_DETACH:
            Shutdown();
            break;
        default: 
            break;
    }
    return TRUE;
}
