#include "Game/Game.hpp"

#include <thread>
#include <memory>
#include <Windows.h>

std::unique_ptr<Game> SR = nullptr;
std::atomic<bool> Running = true;
std::thread Thread;

void Init()
{
    SR = std::make_unique<Game>();

    while (Running)
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved) 
{
    switch (dwReason) 
    {
        case DLL_PROCESS_ATTACH:
            Thread = std::thread(Init);
            break;
        case DLL_THREAD_ATTACH:
            break;
        case DLL_THREAD_DETACH:
            break;
        case DLL_PROCESS_DETACH:
            Running = false;
            Thread.join();
            break;
        default: 
            break;
    }
    return TRUE;
}
