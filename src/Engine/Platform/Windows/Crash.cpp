#include "Base.hpp"

#include "Engine/Core/Memory/Memory.hpp"
#include "Engine/Core/System/Crash.hpp"
#include "Engine/Core/System/Environment.hpp"

#include <sentry.h>
#include <csignal>
#include <exception>
#include <fstream>

namespace IzEngine
{
	// How long a dump copied out of the database is kept. Crashpad has its own policy for the
	// database itself; this is only about the copies.
	constexpr int RetentionDays = 30;

	// The item header crashpad writes in front of the dump when it packs a crash for the reporter.
	constexpr std::string_view MinidumpMarker = "\"attachment_type\":\"event.minidump\"";

	static LPTOP_LEVEL_EXCEPTION_FILTER WINAPI LockedSetUnhandledExceptionFilter(LPTOP_LEVEL_EXCEPTION_FILTER)
	{
		return nullptr;
	}

	static std::filesystem::path ReporterPath()
	{
		return Environment::Path(Directory::Bin) / "CrashReporter.exe";
	}

	static bool HasMinidump(const std::filesystem::path& envelope)
	{
		std::ifstream file(envelope, std::ios::binary);
		const std::string data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		return data.find(MinidumpMarker) != std::string::npos;
	}

	void Crash::Initialize()
	{
		// SEM_NOGPFAULTERRORBOX is deliberately not set: it turns Windows Error Reporting off for
		// the process, and the WER module is what catches the fast fail crashes that never reach an
		// exception filter at all.
		SetErrorMode(SEM_FAILCRITICALERRORS);

		// Reserve stack so the crash path can still run after a stack overflow.
		ULONG stackGuarantee = 32 * 1024;
		SetThreadStackGuarantee(&stackGuarantee);

		// Route the CRT failure paths (abort, terminate, pure virtual call) into an exception, which
		// is the only thing the handler sees.
		signal(SIGABRT, [](int) { RaiseException(0xE0000001, EXCEPTION_NONCONTINUABLE, 0, nullptr); });
		std::set_terminate([] { RaiseException(0xE0000002, EXCEPTION_NONCONTINUABLE, 0, nullptr); });

		// Housekeeping must never keep the handler from starting: the range-for increments and the
		// narrow filenames in there throw on a folder another instance removes mid-walk or an odd name.
		try
		{
			Sweep();
		}
		catch (const std::exception& e)
		{
			Log::WriteLine(Channel::Warning, "Skipped the crash report cleanup: {}", e.what());
		}

		const auto database = Environment::Path(Directory::Reports) / ".sentry";
		const auto handler = Environment::Path(Directory::Bin) / "crashpad_handler.exe";
		const auto reporter = ReporterPath();

		sentry_options_t* options = sentry_options_new();
		sentry_options_set_database_pathw(options, database.wstring().c_str());
		sentry_options_set_handler_pathw(options, handler.wstring().c_str());
		sentry_options_set_release(options, APPLICATION_ID "@" APPLICATION_VERSION);

		// Sending is the reporter's job: crashpad hands it the dump instead of uploading, and it
		// only sends when the player clicks. Sessions are the one thing sentry would still send on
		// its own, so they are off. Without the reporter binary crashpad would launch nothing and
		// then delete the report, so the DSN goes with it and a build without it keeps its dumps
		// locally. Measured: with no DSN crashpad still starts and still writes the dump.
		sentry_options_set_auto_session_tracking(options, 0);
		if (std::filesystem::exists(reporter))
		{
			sentry_options_set_external_crash_reporter_pathw(options, reporter.wstring().c_str());
			if (!std::string_view(SENTRY_DSN).empty())
				sentry_options_set_dsn(options, SENTRY_DSN);
		}

		Active = sentry_init(options) == 0;

		// Crashpad owns the exception filter from here, so stop the game from replacing it.
		if (Active)
			Patch(reinterpret_cast<uintptr_t>(GetModuleHandle(nullptr)));
	}

	void Crash::Shutdown()
	{
		if (Active)
			sentry_close();
	}

	void Crash::Sweep()
	{
		const auto reports = Environment::Path(Directory::Reports);
		const auto database = reports / ".sentry";
		const auto reporter = ReporterPath();
		const auto now = std::filesystem::file_time_type::clock::now();
		std::error_code ec;

		// A dump of a loaded game runs to tens of megabytes, and nothing else ever deletes these:
		// crashpad prunes its own database, not the copies taken out of it.
		for (const auto& entry : std::filesystem::directory_iterator(reports, ec))
		{
			if (!entry.path().filename().string().ends_with("_minidump.dmp"))
				continue;

			const auto written = entry.last_write_time(ec);
			if (!ec && now - written > std::chrono::days(RetentionDays))
				std::filesystem::remove(entry.path(), ec);
		}

		// Without the reporter crashpad keeps the dumps itself, named by its own uuid and buried in
		// the database layout, so each one is copied out under the name the tooling reads.
		for (const auto& entry : std::filesystem::recursive_directory_iterator(database, ec))
		{
			if (entry.path().extension() != ".dmp")
				continue;

			const auto copy = reports / (entry.path().stem().string() + "_minidump.dmp");
			if (!std::filesystem::exists(copy, ec))
				std::filesystem::copy_file(entry.path(), copy, ec);
		}

		// Crashpad packs each crash into an envelope for the reporter and leaves the file to it, so
		// one still here holds a dump the reporter never finished with: offer it again. The rest
		// are the header-only envelopes sentry pre-writes for every run, dropped once old enough
		// that no running instance can still own one.
		for (const auto& entry : std::filesystem::directory_iterator(database / "external", ec))
		{
			if (entry.path().extension() != ".envelope")
				continue;

			if (HasMinidump(entry.path()))
			{
				if (std::filesystem::exists(reporter, ec))
					Report(reporter, entry.path());
				continue;
			}

			const auto written = entry.last_write_time(ec);
			if (!ec && now - written > std::chrono::days(RetentionDays))
				std::filesystem::remove(entry.path(), ec);
		}
	}

	void Crash::Patch(uintptr_t base)
	{
		Memory::PatchImport(base, "kernel32.dll", "SetUnhandledExceptionFilter",
			reinterpret_cast<void*>(&LockedSetUnhandledExceptionFilter));
		Memory::PatchImport(base, "KERNELBASE.dll", "SetUnhandledExceptionFilter",
			reinterpret_cast<void*>(&LockedSetUnhandledExceptionFilter));
	}

	// The same call crashpad makes: the reporter takes the envelope path and owns the file from there.
	void Crash::Report(const std::filesystem::path& reporter, const std::filesystem::path& envelope)
	{
		std::wstring command = std::format(L"\"{}\" \"{}\"", reporter.wstring(), envelope.wstring());

		STARTUPINFOW startup{ sizeof(startup) };
		PROCESS_INFORMATION process{};
		if (!CreateProcessW(reporter.c_str(), command.data(), nullptr, nullptr, FALSE, DETACHED_PROCESS, nullptr,
				nullptr, &startup, &process))
			return;

		CloseHandle(process.hProcess);
		CloseHandle(process.hThread);
	}
}
