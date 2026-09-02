#include "Base.hpp"

#include "Engine/Core/Memory/Memory.hpp"
#include "Engine/Core/System/Crash.hpp"
#include "Engine/Core/System/Environment.hpp"

#include <sentry.h>
#include <csignal>
#include <exception>

namespace IzEngine
{
	// How long a dump copied out of the database is kept. Crashpad has its own policy for the
	// database itself; this is only about the copies.
	constexpr int RetentionDays = 30;

	static LPTOP_LEVEL_EXCEPTION_FILTER WINAPI LockedSetUnhandledExceptionFilter(LPTOP_LEVEL_EXCEPTION_FILTER)
	{
		return nullptr;
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

		Sweep();

		const auto database = Environment::Path(Directory::Reports) / ".sentry";
		const auto handler = Environment::Path(Directory::Bin) / "crashpad_handler.exe";

		sentry_options_t* options = sentry_options_new();

		// Measured: with no DSN crashpad still starts and still writes the dump, so a build from a
		// fork keeps its crash reports locally and only loses the upload. That is worth having, so
		// the handler comes up either way and only the consent prompt is tied to the DSN.
		Uploads = !std::string_view(SENTRY_DSN).empty();
		if (Uploads)
			sentry_options_set_dsn(options, SENTRY_DSN);

		sentry_options_set_database_pathw(options, database.wstring().c_str());
		sentry_options_set_handler_pathw(options, handler.wstring().c_str());
		sentry_options_set_release(options, APPLICATION_ID "@" APPLICATION_VERSION);

		// Nothing is uploaded before the player has answered the prompt. The dump is still written
		// in the meantime; it simply waits in the database until consent is given or revoked.
		sentry_options_set_require_user_consent(options, 1);
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

	// Nothing to ask, and nothing to offer, when there is nowhere to send: the dumps stay on disk
	// regardless of the answer.
	bool Crash::Available()
	{
		return Active && Uploads;
	}

	bool Crash::Answered()
	{
		return !Available() || sentry_user_consent_get() != SENTRY_USER_CONSENT_UNKNOWN;
	}

	bool Crash::Sending()
	{
		return Available() && sentry_user_consent_get() == SENTRY_USER_CONSENT_GIVEN;
	}

	void Crash::Consent(bool allow)
	{
		if (!Active)
			return;

		if (allow)
			sentry_user_consent_give();
		else
			sentry_user_consent_revoke();
	}

	void Crash::Sweep()
	{
		// Crashpad names dumps by its own uuid, buries them in the database layout and prunes them
		// on its own schedule, so each one is copied out under the name the tooling reads.
		const auto reports = Environment::Path(Directory::Reports);
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

		for (const auto& entry : std::filesystem::recursive_directory_iterator(reports / ".sentry", ec))
		{
			if (entry.path().extension() != ".dmp")
				continue;

			const auto copy = reports / (entry.path().stem().string() + "_minidump.dmp");
			if (!std::filesystem::exists(copy, ec))
				std::filesystem::copy_file(entry.path(), copy, ec);
		}
	}

	void Crash::Patch(uintptr_t base)
	{
		Memory::PatchImport(base, "kernel32.dll", "SetUnhandledExceptionFilter",
			reinterpret_cast<void*>(&LockedSetUnhandledExceptionFilter));
		Memory::PatchImport(base, "KERNELBASE.dll", "SetUnhandledExceptionFilter",
			reinterpret_cast<void*>(&LockedSetUnhandledExceptionFilter));
	}
}
