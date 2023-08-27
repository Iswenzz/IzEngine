#include <iostream>
#include <thread>
#include <memory>
#include <windows.h>
#include <MinHook.h>

const DWORD COD4XEntry = reinterpret_cast<DWORD>(GetModuleHandleA("cod4x_021.dll"));

template <typename T>
class Hook
{
public:
    uintptr_t Address = 0;
    uintptr_t Detour = 0;
    uintptr_t Trampoline = 0;

    Hook() = default;

    Hook(uintptr_t address, T detour)
    {
        Address = address;
        Detour = reinterpret_cast<uintptr_t>(detour);

        Install();
    }

    T Install()
    {
        LPVOID lpAddress = reinterpret_cast<LPVOID>(Address);
        LPVOID lpDetour = reinterpret_cast<LPVOID>(Detour);
        LPVOID lpTrampoline = nullptr;

        MH_CreateHook(lpAddress, lpDetour, &lpTrampoline);
        Enable();

        Trampoline = reinterpret_cast<uintptr_t>(lpTrampoline);
        return reinterpret_cast<T>(lpTrampoline);
    }

    void Remove()
    {
        std::cout << "Remove" << std::endl;
        MH_RemoveHook(reinterpret_cast<LPVOID>(Address));
    }

    void Enable()
    {
        MH_EnableHook(reinterpret_cast<LPVOID>(Address));
    }

    void Disable()
    {
        MH_DisableHook(reinterpret_cast<LPVOID>(Address));
    }

    template <typename R = void, typename ...Args>
    R Call(const Args... args)
    {
        return reinterpret_cast<T>(Trampoline)(args...);
    }
};

class Memory
{
public:
    static inline void Write(uintptr_t address, std::string bytes)
    {
        DWORD oldProtect;
        LPVOID lpAddress = reinterpret_cast<LPVOID>(address);
        int size = bytes.length();

        VirtualProtect(lpAddress, size, PAGE_EXECUTE_READWRITE, &oldProtect);
        memcpy(lpAddress, reinterpret_cast<const void*>(bytes.c_str()), size);
        VirtualProtect(lpAddress, size, oldProtect, NULL);
    }
};

typedef void (*Com_PrintMessage_t)(int channel, const char* msg, int type);
Hook<Com_PrintMessage_t> Com_PrintMessage_h;

void Com_PrintMessage(int channel, const char* msg, int type)
{
    std::cout << msg;
    Com_PrintMessage_h.Call(channel, msg, type);
}

class Console
{
public:
    Console()
    {
        AllocConsole();

        freopen_s(reinterpret_cast<FILE**>(stdin), "CONIN$", "r", stdin);
        freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);
        freopen_s(reinterpret_cast<FILE**>(stderr), "CONOUT$", "w", stderr);

        Com_PrintMessage_h = Hook<Com_PrintMessage_t>(0x4FCA50, Com_PrintMessage);
    }

    ~Console()
    {
        Com_PrintMessage_h.Remove();
        FreeConsole();
    }
};

void Shutdown()
{
    MH_Uninitialize();
}

void Init()
{
    MH_Initialize();

    std::unique_ptr<Console> console = std::make_unique<Console>();

    uintptr_t antiHook = COD4XEntry + 0x43580;
    uintptr_t aimAssist = 0x452BFA;
    uintptr_t localTagMatrix = 0x434200;

    Memory::Write(antiHook, "\xC3");
    Memory::Write(aimAssist, "\xE8\xA1\xF9\xFA\xFF");
    Memory::Write(localTagMatrix, "\x51\x53\x8B\x5C\x24");

    while (true)
        Sleep(500);
    Shutdown();
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
