#include "Browser.hpp"

#include "Engine/Core/System/Environment.hpp"

namespace IzEngine
{
	void Browser::Initialize(bool multithreaded)
	{
		IZ_ASSERT(!Active, "Browser already initialized.");

		CefMainArgs args(GetModuleHandle(nullptr));
		App = new BrowserApp();
		Multithreaded = multithreaded;
		int code = CefExecuteProcess(args, App, nullptr);
		if (code >= 0)
			return;

		const auto pathResources = Environment::Path(Directory::Bin);
		const auto pathLocales = Environment::Path(Directory::Bin);
		const auto pathCache = Environment::Path(Directory::App) / "Cache";
		const auto pathLog = Environment::Path(Directory::App) / "Logs" / "cef.log";
		const auto pathSubProcess = Environment::Path(Directory::Bin) / "CEF.exe";

		CefSettings settings;
		settings.log_severity = LOGSEVERITY_DISABLE;
		settings.multi_threaded_message_loop = Multithreaded;
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

		Active = true;
	}

	void Browser::Shutdown()
	{
		IZ_ASSERT(Active, "Browser already shutdown.");

		for (auto& instance : Instances)
			Stop(instance);

		Instances.clear();
		CefShutdown();
		Active = false;
	}

	Ref<BrowserInstance> Browser::Add(const std::string& id, const std::string& url, const vec2& position,
		const vec2& size, const vec2& frameSize)
	{
		IZ_ASSERT(!Get(id), "Browser instance already exists");

		auto instance = CreateRef<BrowserInstance>();
		instance->ID = id;
		instance->URL = url;
		instance->Position = position;
		instance->Size = size;
		instance->FrameSize = frameSize;

		Instances.push_back(instance);
		Start(instance);
		return instance;
	}

	void Browser::Remove(const std::string& id)
	{
		auto it = std::ranges::find_if(Instances, [&](const Ref<BrowserInstance>& i) { return i->ID == id; });
		if (it == Instances.end())
			return;

		Stop(*it);
		Instances.erase(it);
	}

	Ref<BrowserInstance> Browser::Get(const std::string& id)
	{
		auto it = std::ranges::find_if(Instances, [&](const Ref<BrowserInstance>& i) { return i->ID == id; });
		return it != Instances.end() ? *it : nullptr;
	}

	void Browser::Start(const Ref<BrowserInstance>& instance)
	{
		if (!Active || instance->Open)
			return;

		instance->Client = new BrowserClient(instance);

		CefBrowserSettings browserSettings;
		browserSettings.windowless_frame_rate = 120;

		CefWindowInfo windowInfo;
		windowInfo.SetAsWindowless(nullptr);

		CefBrowserHost::CreateBrowser(windowInfo, instance->Client, instance->URL, browserSettings, nullptr, nullptr);

		if (Multithreaded)
		{
			auto start = std::chrono::steady_clock::now();
			while (!instance->Client->IsOpened())
			{
				auto elapsed = std::chrono::steady_clock::now() - start;
				if (elapsed > std::chrono::seconds(3))
					return;
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
		}
		else
		{
			CefDoMessageLoopWork();
		}
		instance->Open = true;
	}

	void Browser::Stop(const Ref<BrowserInstance>& instance)
	{
		if (!Active || !instance->Open)
			return;

		if (!instance->Browser)
		{
			instance->Client = nullptr;
			instance->Open = false;
			return;
		}
		instance->Browser->GetHost()->CloseBrowser(true);
		instance->Browser = nullptr;

		if (Multithreaded)
		{
			auto start = std::chrono::steady_clock::now();
			while (!instance->Client->IsClosed())
			{
				auto elapsed = std::chrono::steady_clock::now() - start;
				if (elapsed > std::chrono::seconds(3))
					break;
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
		}
		else
		{
			CefDoMessageLoopWork();
		}
		instance->Client = nullptr;
		instance->Open = false;
	}

	void Browser::Frame(const Ref<BrowserInstance>& instance)
	{
		if (!instance->Open || !instance->Show || !instance->Browser)
			return;

		auto host = instance->Browser->GetHost();
		vec2 position = instance->Position;
		vec2 size = instance->Size;
		UI::Screen.Apply(position, size, Horizontal::Left, Vertical::Top);

		uint32_t modifiers = 0;
		if (Input::IsDown(Key_Ctrl) || Input::IsDown(Key_RightCtrl))
			modifiers |= EVENTFLAG_CONTROL_DOWN;
		if (Input::IsDown(Key_Shift) || Input::IsDown(Key_RightShift))
			modifiers |= EVENTFLAG_SHIFT_DOWN;
		if (Input::IsDown(Key_Alt) || Input::IsDown(Key_RightAlt))
			modifiers |= EVENTFLAG_ALT_DOWN;

		vec2 relative = Mouse::Position - position;
		if (Math::Contains(relative, size))
		{
			CefMouseEvent mouseEvent;
			mouseEvent.x = static_cast<int>(relative.x / size.x * instance->FrameSize.x);
			mouseEvent.y = static_cast<int>(relative.y / size.y * instance->FrameSize.y);
			mouseEvent.modifiers = modifiers;

			host->SendMouseMoveEvent(mouseEvent, false);

			if (Input::IsDown(Button_Left))
				host->SendMouseClickEvent(mouseEvent, MBT_LEFT, false, 1);
			if (Input::IsUp(Button_Left))
				host->SendMouseClickEvent(mouseEvent, MBT_LEFT, true, 1);
			if (Input::IsDown(Button_Right))
				host->SendMouseClickEvent(mouseEvent, MBT_RIGHT, false, 1);
			if (Input::IsUp(Button_Right))
				host->SendMouseClickEvent(mouseEvent, MBT_RIGHT, true, 1);
			if (Mouse::ScrollDelta)
				host->SendMouseWheelEvent(mouseEvent, 0, Mouse::ScrollDelta * 120);
		}

		for (const auto& info : Input::Inputs)
		{
			const InputEnum id = info.ID;
			if (!info.IsRegistered())
				continue;
			if (id == Button_Left || id == Button_Right || id == Button_Middle)
				continue;
			if (!Input::IsDown(id) && !Input::IsUp(id))
				continue;

			CefKeyEvent keyEvent;
			keyEvent.modifiers = modifiers;
			keyEvent.windows_key_code = info.OS;
			keyEvent.native_key_code = info.OS;

			if (Input::IsPressed(id))
			{
				keyEvent.type = KEYEVENT_RAWKEYDOWN;
				host->SendKeyEvent(keyEvent);

				if (Keyboard::Char)
				{
					keyEvent.type = KEYEVENT_CHAR;
					keyEvent.windows_key_code = Keyboard::Char;
					keyEvent.character = Keyboard::Char;
					host->SendKeyEvent(keyEvent);
				}
			}
			if (Input::IsUp(id))
			{
				keyEvent.type = KEYEVENT_KEYUP;
				host->SendKeyEvent(keyEvent);
			}
		}
		std::scoped_lock lock(instance->TextureMutex);
		Draw2D::DrawQuad(vec3(position, 0), size, vec4(0, 0, 0, 1));

		if (instance->Texture)
			Draw2D::DrawQuad(vec3(position, 0), size, 0, instance->Texture, vec4(1));
	}

	void Browser::Frame()
	{
		if (!Multithreaded)
			CefDoMessageLoopWork();

		for (auto& instance : Instances)
			Frame(instance);
	}

	void Browser::Lock()
	{
		if (!TextureLocks.empty())
			return;

		Paused = true;

		for (const auto& instance : Instances)
			if (instance)
				TextureLocks.emplace_back(instance->TextureMutex);
	}

	void Browser::Unlock()
	{
		TextureLocks.clear();
		Paused = false;
	}

	void Browser::SetURL(const Ref<BrowserInstance>& instance, const std::string& url)
	{
		instance->URL = url;

		if (!instance->Browser)
			return;

		if (const auto frame = instance->Browser->GetMainFrame())
			frame->LoadURL(url);
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
		rect = CefRect(0, 0, Instance->FrameSize.x, Instance->FrameSize.y);
	}

	void BrowserClient::OnAfterCreated(CefRefPtr<CefBrowser> browser)
	{
		Instance->Browser = browser;
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
