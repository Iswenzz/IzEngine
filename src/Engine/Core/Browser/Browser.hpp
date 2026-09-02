#pragma once
#include "Engine/Backend/ImGUI/Components/Frame.hpp"
#include "Engine/Core/Common.hpp"
#include "Engine/Renderer/Base/Texture.hpp"

#include <cef_app.h>
#include <cef_browser.h>
#include <cef_client.h>

namespace IzEngine
{
	class BrowserClient;
	struct BrowserInstance;

	class API BrowserFrame : public Frame
	{
	public:
		BrowserInstance* Instance = nullptr;
		vec2 PagePosition;
		vec2 PageSize;
		bool Hovered = false;
		bool Focused = false;

		BrowserFrame();
		virtual ~BrowserFrame() = default;

		void OnRender() override;
	};

	struct BrowserInstance
	{
		std::string ID;
		std::string URL;
		CefRefPtr<CefBrowser> Browser;
		CefRefPtr<BrowserClient> Client;
		Ref<Texture> Texture;
		std::mutex TextureMutex;
		Ref<BrowserFrame> Window = CreateRef<BrowserFrame>();
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

	class BrowserClient : public CefClient,
						  public CefLifeSpanHandler,
						  public CefRenderHandler,
						  public CefContextMenuHandler
	{
	public:
		BrowserClient(const Ref<BrowserInstance>& instance) : Instance(instance) { }

		CefRefPtr<CefRenderHandler> GetRenderHandler() override;
		CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override;
		CefRefPtr<CefContextMenuHandler> GetContextMenuHandler() override;
		void GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) override;

		void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList& dirtyRects,
			const void* buffer, int width, int height) override;
		void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
		void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;
		void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
			CefRefPtr<CefContextMenuParams> params, CefRefPtr<CefMenuModel> model) override;
		bool OnBeforePopup(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int popupId,
			const CefString& targetUrl, const CefString& targetFrameName, WindowOpenDisposition targetDisposition,
			bool userGesture, const CefPopupFeatures& popupFeatures, CefWindowInfo& windowInfo,
			CefRefPtr<CefClient>& client, CefBrowserSettings& settings, CefRefPtr<CefDictionaryValue>& extraInfo,
			bool* noJavascriptAccess) override;

		bool IsOpened();
		bool IsClosed();

	private:
		Ref<BrowserInstance> Instance = nullptr;
		std::atomic<bool> Opened = false;
		std::atomic<bool> Closed = false;

		IMPLEMENT_REFCOUNTING(BrowserClient);
	};

	struct BrowserLock
	{
		Ref<BrowserInstance> Instance;
		std::unique_lock<std::mutex> Lock;
	};

	class API Browser
	{
	public:
		static inline CefRefPtr<BrowserApp> App;
		static inline std::vector<Ref<BrowserInstance>> Instances;
		static inline bool Multithreaded = false;
		static inline bool Active = false;
		static inline std::atomic<bool> Paused = false;

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

		static void Lock();
		static void Unlock();

		static void SetURL(const Ref<BrowserInstance>& instance, const std::string& url);

	private:
		static inline std::vector<BrowserLock> TextureLocks;
		static inline int LockDepth = 0;
	};
}
