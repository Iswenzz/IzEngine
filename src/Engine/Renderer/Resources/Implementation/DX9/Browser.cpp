#include "Renderer/Common.hpp"

#include "Renderer/Resources/Browser.hpp"

#include "Core/System/Environment.hpp"
#include "Core/System/Window.hpp"

namespace IzEngine
{
	void Browser::Initialize()
	{
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

		CefBrowserSettings browserSettings;
		CefWindowInfo windowInfo;
		windowInfo.SetAsWindowless(nullptr);

		Client = new BrowserClient();
		//const auto url = "https://youtu.be/UgQFcvYg9Kk?t=90";
		const auto url = "https://youtube.com";
		CefBrowserHost::CreateBrowser(windowInfo, Client, url, browserSettings, nullptr, nullptr);
	}

	void Browser::Shutdown()
	{
		if (Instance)
		{
			Instance->GetHost()->CloseBrowser(false);
			Instance = nullptr;
		}
		while (!Client->IsClosed())
			std::this_thread::sleep_for(std::chrono::milliseconds(10));

		CefShutdown();
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
		rect = CefRect(0, 0, Window::Size.x, Window::Size.y);
	}

	void BrowserClient::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList& dirtyRects,
		const void* buffer, int width, int height)
	{
		if (!buffer)
			return;

		std::scoped_lock lock(Browser::TextureMutex);

		if (!Browser::Texture)
			Browser::Texture = Texture::Create("browser", Window::Size);

		IDirect3DTexture9* texture = reinterpret_cast<IDirect3DTexture9*>(Browser::Texture->Data);
		D3DLOCKED_RECT rect;
		texture->LockRect(0, &rect, nullptr, 0);
		memcpy(rect.pBits, buffer, width * height * 4);
		texture->UnlockRect(0);
	}

	void BrowserClient::OnAfterCreated(CefRefPtr<CefBrowser> browser)
	{
		Browser::Instance = browser;
	}

	void BrowserClient::OnBeforeClose(CefRefPtr<CefBrowser> browser)
	{
		Closed.store(true);
	}

	bool BrowserClient::IsClosed()
	{
		return Closed.load();
	}
}
