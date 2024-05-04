#include "Windows/Base.hpp"

#include "Core/System/Crash.hpp"
#include "Core/System/Environment.hpp"

#include <DbgHelp.h>
#include <fstream>

namespace IzEngine
{
	void Crash::Setup()
	{
		SetUnhandledExceptionFilter(reinterpret_cast<LPTOP_LEVEL_EXCEPTION_FILTER>(ExceptionHandler));
	}

	void Crash::ExceptionHandler(void *exception)
	{
		EXCEPTION_POINTERS *ex = reinterpret_cast<EXCEPTION_POINTERS *>(exception);
		Log::WriteLine(Channel::Error, "The program has crashed with code: {:X}", ex->ExceptionRecord->ExceptionCode);
		ID = UUID();

		StackTrace(ex);
		MiniDump(ex);
	}

	void Crash::MiniDump(void *exception)
	{
		EXCEPTION_POINTERS *ex = reinterpret_cast<EXCEPTION_POINTERS *>(exception);
		const auto path = Environment::ReportsDirectory / (ID.String + "_minidump.dmp");
		const auto wpath = path.wstring();
		HANDLE file = CreateFileW(wpath.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if (file != INVALID_HANDLE_VALUE)
		{
			MINIDUMP_EXCEPTION_INFORMATION info = { 0 };
			info.ThreadId = GetCurrentThreadId();
			info.ExceptionPointers = ex;
			info.ClientPointers = false;

			Log::WriteLine(Channel::Info, "Minidump: {}", path.string());
			MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), file, MiniDumpNormal, &info, NULL, NULL);
		}
		CloseHandle(file);
	}

	void Crash::StackTrace(void *exception)
	{
		EXCEPTION_POINTERS *ex = reinterpret_cast<EXCEPTION_POINTERS *>(exception);

		HANDLE process = GetCurrentProcess();
		HANDLE thread = GetCurrentThread();
		CONTEXT *ctx = ex->ContextRecord;
		CONTEXT copy = *ctx;
		DWORD displacement = 0;
		DWORD64 displacement64 = 0;
		int machine = 0;

		char buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)] = { 0 };
		PSYMBOL_INFO symbol = reinterpret_cast<PSYMBOL_INFO>(buffer);

		std::string module;
		module.reserve(256);

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

		const auto path = Environment::ReportsDirectory / (ID.String + "_stacktrace.log");
		Log::WriteLine(Channel::Info, "Stacktrace: {}", path.string());
		std::ofstream file(path);

		for (int frame = 0;; frame++)
		{
			bool result = StackWalk64(machine, process, thread, &stack, &copy, nullptr, SymFunctionTableAccess64,
				SymGetModuleBase64, nullptr);
			if (!result || !stack.AddrPC.Offset)
				break;

			symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
			symbol->MaxNameLen = MAX_SYM_NAME;
			SymFromAddr(process, stack.AddrPC.Offset, &displacement64, symbol);

			IMAGEHLP_LINE64 line = { 0 };
			line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

			HMODULE hModule = nullptr;
			module.clear();
			GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
				reinterpret_cast<LPCSTR>(stack.AddrPC.Offset), &hModule);
			GetModuleFileName(hModule, module.data(), module.capacity());

			std::string traceLine;
			if (SymGetLineFromAddr64(process, stack.AddrPC.Offset, &displacement, &line))
				traceLine = std::format(" {}+{} ", line.FileName, line.LineNumber);

			file << std::format("{:>5}: {}({}){}[0x{:X}]\n", frame, module, symbol->Name, traceLine, symbol->Address);
		}
		file.close();
		SymCleanup(process);
	}
}
