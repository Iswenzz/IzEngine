#include "Windows/Base.hpp"

#include "Core/System/System.hpp"

#include <TlHelp32.h>
#include <shellapi.h>

namespace IzEngine
{
	void System::MapProcesses()
	{
		Processes.clear();

		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
		if (hSnapshot == INVALID_HANDLE_VALUE)
			return;

		PROCESSENTRY32 entry = { 0 };
		entry.dwSize = sizeof(PROCESSENTRY32);
		if (Process32First(hSnapshot, &entry))
		{
			do
			{
				Processes[entry.szExeFile] = entry.th32ProcessID;
			} while (Process32Next(hSnapshot, &entry));
		}
		CloseHandle(hSnapshot);
	}

	void System::MapModules(uintptr_t process) 
	{
		Modules.clear();

		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, process);
		if (hSnapshot == INVALID_HANDLE_VALUE)
			return;

		MODULEENTRY32 entry = { 0 };
		entry.dwSize = sizeof(MODULEENTRY32);
		if (Module32First(hSnapshot, &entry))
		{
			do
			{
				Modules[entry.szModule] = reinterpret_cast<uintptr_t>(entry.modBaseAddr);
			} while (Module32Next(hSnapshot, &entry));
		}
		CloseHandle(hSnapshot);
	}

	void System::Shell(const std::string& command)
	{
		ShellExecute(nullptr, "open", command.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
	}

	bool System::IsDebug()
	{
#ifdef _DEBUG
		return true;
#else
		return false;
#endif
	}
}
