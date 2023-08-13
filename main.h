#pragma once

#include <iostream>
#include <windows.h>

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  dwReason, LPVOID lpReserved);
void OnAttach();