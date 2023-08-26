#include <iostream>
#include <thread>
#include <windows.h>
#include <MinHook.h>

const DWORD COD4XEntry = reinterpret_cast<DWORD>(GetModuleHandleA("cod4x_021.dll"));

typedef void (*Com_PrintMessage_t)(int channel, const char* msg, int type);
Com_PrintMessage_t Com_PrintMessage_f = nullptr;
LPVOID Com_PrintMessage_a = reinterpret_cast<LPVOID*>(0x4FCA50);

void Com_PrintMessage(int channel, const char* msg, int type)
{
    std::cout << msg;
    Com_PrintMessage_f(channel, msg, type);
}

void Init()
{
    MH_Initialize();
    AllocConsole();

    freopen_s(reinterpret_cast<FILE**>(stdin), "CONIN$", "r", stdin);
    freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);
    freopen_s(reinterpret_cast<FILE**>(stderr), "CONOUT$", "w", stderr);

    LPVOID antiHook = reinterpret_cast<LPVOID>(COD4XEntry + 0x43580);

    DWORD oldProtect;
    VirtualProtect(antiHook, 1, PAGE_EXECUTE_READWRITE, &oldProtect);
    memcpy(antiHook, "\xC3", 1);
    VirtualProtect(antiHook, 1, oldProtect, NULL);

    MH_CreateHook(Com_PrintMessage_a, Com_PrintMessage, reinterpret_cast<LPVOID*>(&Com_PrintMessage_f));
    MH_EnableHook(Com_PrintMessage_a);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  dwReason, LPVOID lpReserved) 
{
    switch (dwReason) 
    {
        case DLL_PROCESS_ATTACH:
            std::thread(Init).detach();
            break;
        case DLL_THREAD_ATTACH:
            break;
        case DLL_THREAD_DETACH:
            break;
        case DLL_PROCESS_DETACH:
            break;
        default: 
            break;
    }
    return TRUE;
}
