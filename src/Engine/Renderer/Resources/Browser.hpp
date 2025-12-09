#pragma once
#include "Core/Common.hpp"

#include "Texture.hpp"

#include <cef_app.h>
#include <cef_browser.h>
#include <cef_client.h>
#include <latch>

namespace IzEngine
{
	class BrowserApp : public CefApp
	{
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
		static inline Ref<Texture> Texture;
		static inline std::mutex TextureMutex;
		static inline vec2 Size = { 1280, 720 };
		static inline bool Active = false;
		static inline bool Open = false;

		static void Initialize();
		static void Shutdown();

		static void Start();
		static void Stop();

		static void SetURL(const std::string& url);
	};
}
