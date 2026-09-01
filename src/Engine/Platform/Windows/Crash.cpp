#include "Base.hpp"

#include "Engine/Core/Memory/Memory.hpp"
#include "Engine/Core/System/Crash.hpp"
#include "Engine/Core/System/Environment.hpp"

#include <DbgHelp.h>
#include <csignal>
#include <exception>
#include <fstream>

namespace IzEngine
{
	static LPTOP_LEVEL_EXCEPTION_FILTER WINAPI LockedSetUnhandledExceptionFilter(LPTOP_LEVEL_EXCEPTION_FILTER)
	{
		return nullptr;
	}

	void Crash::Setup()
	{
		SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX);
		SetUnhandledExceptionFilter(reinterpret_cast<LPTOP_LEVEL_EXCEPTION_FILTER>(&Crash::ExceptionHandler));

		// Reserve stack so the handler can still run after a stack overflow.
		ULONG stackGuarantee = 32 * 1024;
		SetThreadStackGuarantee(&stackGuarantee);

		// Route CRT failure paths (abort, terminate, pure virtual call, invalid parameter)
		// into the same handler so they also produce dumps.
		signal(SIGABRT, [](int) { RaiseException(0xE0000001, EXCEPTION_NONCONTINUABLE, 0, nullptr); });
		std::set_terminate([] { RaiseException(0xE0000002, EXCEPTION_NONCONTINUABLE, 0, nullptr); });

		// Prevent anyone from replacing the filter after us.
		Patch(reinterpret_cast<uintptr_t>(GetModuleHandle(nullptr)));
	}

	long Crash::ExceptionHandler(void* exception)
	{
		// A crash inside the handler must not recurse forever.
		if (Handling.test_and_set())
			TerminateProcess(GetCurrentProcess(), static_cast<UINT>(-2));

		EXCEPTION_POINTERS* ex = reinterpret_cast<EXCEPTION_POINTERS*>(exception);
		ID = UUID();

		// Write the minidump first: it only needs a file handle and dbghelp,
		// while logging/stacktrace allocate and can fail on a corrupted heap.
		MiniDump(ex);
		StackTrace(ex);
		Log::WriteLine(Channel::Error, "The program has crashed with code: {:X}",
			ex->ExceptionRecord ? ex->ExceptionRecord->ExceptionCode : 0);

		// exit() runs atexit/static destructors in a corrupted process and can
		// hang or crash again before the dump is flushed.
		TerminateProcess(GetCurrentProcess(), static_cast<UINT>(-1));
		return EXCEPTION_EXECUTE_HANDLER;
	}

	void Crash::MiniDump(void* exception)
	{
		EXCEPTION_POINTERS* ex = reinterpret_cast<EXCEPTION_POINTERS*>(exception);
		const auto path = Environment::Path(Directory::Reports) / (ID.String + "_minidump.dmp");
		const auto wpath = path.wstring();
		HANDLE file = CreateFileW(wpath.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if (file == INVALID_HANDLE_VALUE)
			return;

		MINIDUMP_EXCEPTION_INFORMATION info = { 0 };
		info.ThreadId = GetCurrentThreadId();
		info.ExceptionPointers = ex;
		info.ClientPointers = false;

		MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), file,
			static_cast<MINIDUMP_TYPE>(MiniDumpNormal | MiniDumpWithIndirectlyReferencedMemory), &info, NULL, NULL);
		FlushFileBuffers(file);
		CloseHandle(file);

		Log::WriteLine(Channel::Info, "Minidump: {}", path.string());
	}

	void Crash::StackTrace(void* exception)
	{
		EXCEPTION_POINTERS* ex = reinterpret_cast<EXCEPTION_POINTERS*>(exception);
		HANDLE process = GetCurrentProcess();
		HANDLE thread = GetCurrentThread();
		CONTEXT* ctx = ex->ContextRecord;
		if (!ctx)
			return;
		CONTEXT copy = *ctx;
		DWORD displacement = 0;
		DWORD64 displacement64 = 0;
		int machine = 0;
		int frame = 0;

		char buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)] = { 0 };
		PSYMBOL_INFO symbol = reinterpret_cast<PSYMBOL_INFO>(buffer);

		char module[MAX_PATH] = { 0 };

		STACKFRAME64 stack = { 0 };
		stack.AddrPC.Mode = AddrModeFlat;
		stack.AddrFrame.Mode = AddrModeFlat;
		stack.AddrStack.Mode = AddrModeFlat;
#if defined(PLATFORM_ARCH_X86)
	#if defined(PLATFORM_32)
		stack.AddrPC.Offset = ctx->Eip;
		stack.AddrFrame.Offset = ctx->Ebp;
		stack.AddrStack.Offset = ctx->Esp;
		machine = IMAGE_FILE_MACHINE_I386;
	#elif defined(PLATFORM_64)
		stack.AddrPC.Offset = ctx->Rip;
		stack.AddrFrame.Offset = ctx->Rbp;
		stack.AddrStack.Offset = ctx->Rsp;
		machine = IMAGE_FILE_MACHINE_AMD64;
	#endif
#elif defined(PLATFORM_ARCH_ARM)
	#if defined(PLATFORM_64)
		stack.AddrPC.Offset = ctx->Pc;
		stack.AddrFrame.Offset = ctx->Fp;
		stack.AddrStack.Offset = ctx->Sp;
		machine = IMAGE_FILE_MACHINE_ARM64;
	#endif
#else
	#error Unsupported platform
#endif
		SymSetOptions(SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS);
		SymInitialize(process, nullptr, true);

		const auto path = Environment::Path(Directory::Reports) / (ID.String + "_stacktrace.log");
		std::ofstream file(path);

		// Written into the report rather than only logged: the log file is a debug build only, so
		// this is where the code has to be for a crash that happened on a player machine.
		const EXCEPTION_RECORD* record = ex->ExceptionRecord;
		file << std::format("Exception 0x{:X} at 0x{:X}\n\n", record ? record->ExceptionCode : 0,
			record ? reinterpret_cast<uintptr_t>(record->ExceptionAddress) : 0);

		while (StackWalk64(machine, process, thread, &stack, &copy, nullptr, SymFunctionTableAccess64,
			SymGetModuleBase64, nullptr))
		{
			symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
			symbol->MaxNameLen = MAX_SYM_NAME;
			SymFromAddr(process, stack.AddrPC.Offset, &displacement64, symbol);

			DWORD64 moduleBase = SymGetModuleBase64(process, stack.AddrPC.Offset);
			module[0] = 0;
			GetModuleFileNameA(reinterpret_cast<HMODULE>(moduleBase), module, sizeof(module));

			IMAGEHLP_LINE64 line = { 0 };
			line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
			std::string traceLine;

			if (SymGetLineFromAddr64(process, stack.AddrPC.Offset, &displacement, &line))
				traceLine = std::format(" {}+{} ", line.FileName, line.LineNumber);

			file << std::format("{:>5}: {}.{}{}[0x{:X}]\n", frame++, module, symbol->Name, traceLine, symbol->Address);

			if (frame > 256)
				break;
		}
		file.flush();
		SymCleanup(process);

		Log::WriteLine(Channel::Info, "Stacktrace: {}", path.string());
	}

	void Crash::Patch(uintptr_t base)
	{
		Memory::PatchImport(base, "kernel32.dll", "SetUnhandledExceptionFilter",
			reinterpret_cast<void*>(&LockedSetUnhandledExceptionFilter));
		Memory::PatchImport(base, "KERNELBASE.dll", "SetUnhandledExceptionFilter",
			reinterpret_cast<void*>(&LockedSetUnhandledExceptionFilter));
	}
}
