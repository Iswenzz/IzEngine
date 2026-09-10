#include "Report.hpp"
#include "Reporter.hpp"

#include "Engine/Core/Network/HTTP.hpp"

void Application::Prepare() { }
void Application::Initialize() { }
void Application::Shutdown() { }

void Application::Dispatch(Event& event)
{
	UI::Dispatch(event);
}

// Crashpad launches this with the envelope it packed the crash into, and the game's sweep does the
// same for one a previous run left behind. An envelope without a dump is the header sentry
// pre-writes for every run: nothing to show, and loading it is what deletes it.
static int Run(const std::filesystem::path& envelope)
{
	// The reporter lives in <root>/<app>/Bin and the engine's fonts in <root>/<app>/Resources.
	Environment::Root(Environment::Executable().parent_path().parent_path().parent_path());

	// The UI saves its layout on shutdown, and that file belongs to the game.
	Environment::Redirect(Directory::Configs, Environment::Path(Directory::Reports) / ".reporter");

	IzEngine::CrashReport report;
	if (!IzEngine::Report::Load(envelope, report))
		return 0;

	ThreadPool::Initialize(1);
	HTTP::Initialize();
	const int code = IzEngine::Reporter::Run(report);
	HTTP::Shutdown();
	ThreadPool::Shutdown();
	return code;
}

#ifdef PLATFORM_WINDOWS
int wmain(int argc, wchar_t* argv[])
{
	return argc < 2 ? 1 : Run(argv[1]);
}
#else
int main(int argc, char* argv[])
{
	return argc < 2 ? 1 : Run(argv[1]);
}
#endif
