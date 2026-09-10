#include "Base.hpp"

#include "Engine/Core/System/System.hpp"

#include <TlHelp32.h>
#include <shellapi.h>

namespace IzEngine
{
	std::map<std::string, uintptr_t> System::MapProcesses()
	{
		std::map<std::string, uintptr_t> map = {};

		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
		if (hSnapshot == INVALID_HANDLE_VALUE)
			return map;

		PROCESSENTRY32 entry = { 0 };
		entry.dwSize = sizeof(PROCESSENTRY32);
		if (Process32First(hSnapshot, &entry))
		{
			do
			{
				map[entry.szExeFile] = entry.th32ProcessID;
			} while (Process32Next(hSnapshot, &entry));
		}
		CloseHandle(hSnapshot);
		return map;
	}

	std::map<std::string, uintptr_t> System::MapModules(uintptr_t process)
	{
		std::map<std::string, uintptr_t> map = {};

		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, process);
		if (hSnapshot == INVALID_HANDLE_VALUE)
			return map;

		MODULEENTRY32 entry = { 0 };
		entry.dwSize = sizeof(MODULEENTRY32);
		if (Module32First(hSnapshot, &entry))
		{
			do
			{
				map[entry.szModule] = reinterpret_cast<uintptr_t>(entry.modBaseAddr);
			} while (Module32Next(hSnapshot, &entry));
		}
		CloseHandle(hSnapshot);
		return map;
	}

	void System::Shell(const std::string& command)
	{
		ShellExecute(nullptr, "open", command.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
	}

	static std::wstring Widen(const std::string& text)
	{
		std::wstring out(MultiByteToWideChar(CP_UTF8, 0, text.data(), static_cast<int>(text.size()), nullptr, 0), L'\0');
		MultiByteToWideChar(CP_UTF8, 0, text.data(), static_cast<int>(text.size()), out.data(), static_cast<int>(out.size()));
		return out;
	}

	void System::Alert(const std::string& title, const std::string& text)
	{
		MessageBoxW(nullptr, Widen(text).c_str(), Widen(title).c_str(), MB_OK | MB_ICONERROR | MB_TOPMOST | MB_SETFOREGROUND);
	}
}
