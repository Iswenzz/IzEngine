#include "Renderer/Common.hpp"

#include "Renderer/Resources/Browser.hpp"

#include "Core/System/Environment.hpp"
#include "Core/System/Window.hpp"

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

		const auto pathResources = Environment::Path(Directory::Resources) / "cef";
		const auto pathLocales = Environment::Path(Directory::Resources) / "cef" / "locales";
		const auto pathCache = Environment::Path(Directory::App) / "cache";
		const auto pathLog = Environment::Path(Directory::App) / "logs" / "cef.log";
		const auto pathSubProcess = Environment::Path(Directory::Base) / "CEF.exe";

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
		if (!Active || Instance)
			return;

		CefBrowserSettings browserSettings;
		browserSettings.windowless_frame_rate = 120;

		CefWindowInfo windowInfo;
		windowInfo.SetAsWindowless(nullptr);

		CefBrowserHost::CreateBrowser(windowInfo, Client, "about:blank", browserSettings, nullptr, nullptr);

		while (!Client->IsOpened())
			std::this_thread::sleep_for(std::chrono::milliseconds(10));

		Open = true;
	}

	void Browser::Stop()
	{
		if (!Active || !Instance)
			return;

		Instance->GetHost()->CloseBrowser(false);
		Instance = nullptr;

		while (!Client->IsClosed())
			std::this_thread::sleep_for(std::chrono::milliseconds(10));

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

	void BrowserClient::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList& dirtyRects,
		const void* buffer, int width, int height)
	{
		if (!buffer || dirtyRects.empty())
			return;

		std::scoped_lock lock(Browser::TextureMutex);
		if (!Browser::Texture || !Browser::Texture->Data)
			Browser::Texture = Texture::Create("browser", Browser::Size);

		IDirect3DTexture9* texture = reinterpret_cast<IDirect3DTexture9*>(Browser::Texture->Data);

		for (const auto& dirtyRect : dirtyRects)
		{
			D3DLOCKED_RECT lockedRect;
			RECT rect = { dirtyRect.x, dirtyRect.y, dirtyRect.x + dirtyRect.width, dirtyRect.y + dirtyRect.height };
			texture->LockRect(0, &lockedRect, &rect, 0);

			const uint8_t* src = reinterpret_cast<const uint8_t*>(buffer) + (dirtyRect.y * width + dirtyRect.x) * 4;
			uint8_t* dst = reinterpret_cast<uint8_t*>(lockedRect.pBits);

			for (int y = 0; y < dirtyRect.height; ++y)
			{
				std::memcpy(dst, src, dirtyRect.width * 4);
				src += width * 4;
				dst += lockedRect.Pitch;
			}
			texture->UnlockRect(0);
		}
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
