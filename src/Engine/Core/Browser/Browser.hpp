#pragma once
#include "Engine/Core/Common.hpp"
#include "Engine/Renderer/Base/Texture.hpp"

#include <cef_app.h>
#include <cef_browser.h>
#include <cef_client.h>

namespace IzEngine
{
	class BrowserApp : public CefApp
	{
	public:
		void OnBeforeCommandLineProcessing(const CefString& processType,
			CefRefPtr<CefCommandLine> commandLine) override;

		IMPLEMENT_REFCOUNTING(BrowserApp);
	};

	class BrowserClient : public CefClient, public CefLifeSpanHandler, public CefRenderHandler
	{
	public:
		CefRefPtr<CefRenderHandler> GetRenderHandler() override;
		CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override;
		void GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) override;

		void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList& dirtyRects,
			const void* buffer, int width, int height) override;
		void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
		void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;

		bool IsOpened();
		bool IsClosed();

	private:
		std::atomic<bool> Opened;
		std::atomic<bool> Closed;

		IMPLEMENT_REFCOUNTING(BrowserClient);
	};

	class API Browser
	{
	public:
		static inline CefRefPtr<CefBrowser> Instance;
		static inline CefRefPtr<BrowserApp> App;
		static inline CefRefPtr<BrowserClient> Client;
		static inline Ref<Texture> Texture = nullptr;
		static inline std::mutex TextureMutex;
		static inline vec2 FrameSize = { 1920, 1080 };
		static inline vec2 MenuPosition = { 20, 20 };
		static inline vec2 MenuSize = { 500, 300 };
		static inline bool Active = false;
		static inline bool Open = false;

		static void Initialize();
		static void Shutdown();

		static void Start();
		static void Stop();
		static void Frame();

		static void SetURL(const std::string& url);
	};
}
