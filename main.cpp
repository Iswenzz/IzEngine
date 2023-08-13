#include <iostream>
#include <fstream>
#include <windows.h>
#include <psapi.h>
#include <tlhelp32.h>

#include "main.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  dwReason, LPVOID lpReserved) {
    switch (dwReason) {
        case DLL_PROCESS_ATTACH:
//            OnAttach(); // WIP
            MessageBox(nullptr, "Hello, World!", "Message Box", MB_OK);
            break;

        case DLL_THREAD_ATTACH:
            break;

        case DLL_THREAD_DETACH:
            break;

        case DLL_PROCESS_DETACH:
            FreeConsole();
            break;

        default: std::cout << "An Unexpected Error has Occurred." << std::endl; break;
    }

    return TRUE;
}

DWORD GetProcessIdByName(const char* processName) {
    PROCESSENTRY32 processEntry;
    processEntry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (Process32First(snapshot, &processEntry)) {
        while (Process32Next(snapshot, &processEntry)) {
            if (_stricmp(processEntry.szExeFile, processName) == 0) {
                CloseHandle(snapshot);
                return processEntry.th32ProcessID;
            }
        }
    }
    CloseHandle(snapshot);
    return 0;
}

void OnAttach() {
//    system("start cmd"); // This works lol
//    AllocConsole();

//    std::cin.rdbuf(std::ifstream("CONIN$").rdbuf());    // Input
//    std::cout.rdbuf(std::ofstream("CONOUT$").rdbuf());  // Output
//    std::cerr.rdbuf(std::ofstream("CONOUT$").rdbuf());  // Error

//    std::cout << "Console spawned." << std::endl;

// WIP
//    const char* procName = "iw3mp.exe";
//    DWORD PID = GetProcessIdByName(procName);
//
//    // Test
//    if (PID != 0) {
//        MessageBox(nullptr, "Game process found. PID: " + PID, "Message Box", MB_OK);
//        printf("Game process found. PID: %lu\n", PID);
//    } else {
//        printf("Game process not found.\n");
//    }
}
