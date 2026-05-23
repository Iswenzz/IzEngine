#pragma once
#include "Engine/Core/Common.hpp"
#include "Engine/Renderer/Base/Texture.hpp"

#include <cef_app.h>
#include <cef_browser.h>
#include <cef_client.h>

namespace IzEngine
{
	class BrowserClient;

	struct BrowserInstance
	{
		std::string ID;
		std::string URL;
		CefRefPtr<CefBrowser> Browser;
		CefRefPtr<BrowserClient> Client;
		Ref<Texture> Texture;
		std::mutex TextureMutex;
		vec2 Position = { 20.0f, 20.0f };
		vec2 Size = { 500.0f, 300.0f };
		vec2 FrameSize = { 1920.0f, 1080.0f };
		bool Open = false;
		bool Show = true;
	};

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
		BrowserClient(const Ref<BrowserInstance>& instance) : Instance(instance) { }

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
		Ref<BrowserInstance> Instance = nullptr;
		std::atomic<bool> Opened = false;
		std::atomic<bool> Closed = false;

		IMPLEMENT_REFCOUNTING(BrowserClient);
	};

	class API Browser
	{
	public:
		static inline CefRefPtr<BrowserApp> App;
		static inline std::vector<Ref<BrowserInstance>> Instances;
		static inline bool Multithreaded = false;
		static inline bool Active = false;

		static void Initialize(bool multithreaded);
		static void Shutdown();

		static Ref<BrowserInstance> Add(const std::string& id, const std::string& url, const vec2& position,
			const vec2& size, const vec2& frameSize = { 1920.0f, 1080.0f });
		static void Remove(const std::string& id);
		static Ref<BrowserInstance> Get(const std::string& id);

		static void Start(const Ref<BrowserInstance>& instance);
		static void Stop(const Ref<BrowserInstance>& instance);
		static void Frame(const Ref<BrowserInstance>& instance);
		static void Frame();

		static void SetURL(const Ref<BrowserInstance>& instance, const std::string& url);
		static void ReleaseTextures();
		static std::vector<std::unique_lock<std::mutex>> LockTextures();
	};
}
