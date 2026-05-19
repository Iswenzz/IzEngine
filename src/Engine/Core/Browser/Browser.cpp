#include "Browser.hpp"

#include "Engine/Core/System/Environment.hpp"

namespace IzEngine
{
	void Browser::Initialize()
	{
		IZ_ASSERT(!Active, "Browser already initialized.");

		CefMainArgs args(GetModuleHandle(nullptr));
		App = new BrowserApp();
		int code = CefExecuteProcess(args, App, nullptr);
		if (code >= 0)
			return;

		const auto pathResources = Environment::Path(Directory::Bin);
		const auto pathLocales = Environment::Path(Directory::Bin);
		const auto pathCache = Environment::Path(Directory::App) / "Cache";
		const auto pathLog = Environment::Path(Directory::App) / "Logs" / "cef.log";
		const auto pathSubProcess = Environment::Path(Directory::Bin) / "CEF.exe";

		CefSettings settings;
		settings.log_severity = LOGSEVERITY_ERROR;
		settings.multi_threaded_message_loop = true;
		settings.windowless_rendering_enabled = true;
		settings.no_sandbox = true;
		CefString(&settings.resources_dir_path).FromString(pathResources.string());
		CefString(&settings.locales_dir_path).FromString(pathLocales.string());
		CefString(&settings.cache_path).FromString(pathCache.string());
		CefString(&settings.root_cache_path).FromString(pathCache.string());
		CefString(&settings.log_file).FromString(pathLog.string());
		CefString(&settings.browser_subprocess_path).FromString(pathSubProcess.string());

		if (!CefInitialize(args, settings, App, nullptr))
			return;

		Client = new BrowserClient();
		Active = true;
	}

	void Browser::Shutdown()
	{
		IZ_ASSERT(Active, "Browser already shutdown.");

		Stop();
		CefShutdown();
		Active = false;
	}

	void Browser::Start()
	{
		if (!Active || Open)
			return;

		CefBrowserSettings browserSettings;
		browserSettings.windowless_frame_rate = 120;

		CefWindowInfo windowInfo;
		windowInfo.SetAsWindowless(nullptr);

		CefBrowserHost::CreateBrowser(windowInfo, Client, "about:blank", browserSettings, nullptr, nullptr);

		auto start = std::chrono::steady_clock::now();
		while (!Client->IsOpened())
		{
			auto elapsed = std::chrono::steady_clock::now() - start;
			if (elapsed > std::chrono::seconds(3))
				return;

			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		Open = true;
	}

	void Browser::Stop()
	{
		if (!Active || !Open)
			return;

		Instance->GetHost()->CloseBrowser(true);
		Instance = nullptr;

		auto start = std::chrono::steady_clock::now();
		while (!Client->IsClosed())
		{
			auto elapsed = std::chrono::steady_clock::now() - start;
			if (elapsed > std::chrono::seconds(3))
				break;

			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		Open = false;
	}

	void Browser::SetURL(const std::string& url)
	{
		Instance->GetMainFrame()->LoadURL(url);
	}

	void BrowserApp::OnBeforeCommandLineProcessing(const CefString& processType, CefRefPtr<CefCommandLine> commandLine)
	{
		if (processType.empty())
		{
			commandLine->AppendSwitchWithValue("autoplay-policy", "no-user-gesture-required");
			commandLine->AppendSwitch("try-supported-channel-layouts");
		}
	}

	CefRefPtr<CefRenderHandler> BrowserClient::GetRenderHandler()
	{
		return this;
	}

	CefRefPtr<CefLifeSpanHandler> BrowserClient::GetLifeSpanHandler()
	{
		return this;
	}

	void BrowserClient::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect)
	{
		rect = CefRect(0, 0, Browser::Size.x, Browser::Size.y);
	}

	void BrowserClient::OnAfterCreated(CefRefPtr<CefBrowser> browser)
	{
		Browser::Instance = browser;
		Opened.store(true);
		Closed.store(false);
	}

	void BrowserClient::OnBeforeClose(CefRefPtr<CefBrowser> browser)
	{
		Opened.store(false);
		Closed.store(true);
	}

	bool BrowserClient::IsOpened()
	{
		return Opened.load();
	}

	bool BrowserClient::IsClosed()
	{
		return Closed.load();
	}
}
