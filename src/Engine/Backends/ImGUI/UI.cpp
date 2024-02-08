#include "UI.hpp"
#include "Engine/Backends/DX9/Device.hpp"

namespace IW3SR::Engine
{
	UI::UI()
	{
		OpenKey = KeyListener(VK_F10);

		Memory = UC::Memory();
		Modules = UC::Modules();
		Settings = UC::Settings();
		Themes = UC::Themes();

		Environment::Deserialize("UI", *this);
	}

	void UI::Initialize()
	{
		Active = true;

		ImGui::SetAllocatorFunctions(&Allocator, &Free, &Data);
		Context = ImGui::CreateContext();
		PlotContext = ImPlot::CreateContext();
		ImGui_ImplWin32_Init(Sys::Get().MainWindow);
		ImGui_ImplDX9_Init(Device::Get().D3Device);

		InitializeContext();
		Plugins::SetRenderer();

		Themes.Initialize();
	}

	void UI::Release()
	{
		Active = false;

		ImGui_ImplDX9_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext(Context);
		ImPlot::DestroyContext(PlotContext);

		Environment::Serialize("UI", *this);
	}

	void UI::Begin()
	{
		if (!Active || HasBegin) return;

		HasBegin = true;
		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		if (OpenKey.IsPressed())
			Open = !Open;

		Themes.ComputeRainbow();
	}

	void UI::End()
	{
		HasBegin = false;
		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	}

	void UI::InitializeContext()
	{
		auto& UI = UI::Get();
		ImGui::SetAllocatorFunctions(UI::Allocator, UI::Free, UI.Data);
		ImGui::SetCurrentContext(UI.Context);
		ImPlot::SetImGuiContext(UI.Context);
		ImPlot::SetCurrentContext(UI.PlotContext);
	}

	void* UI::Allocator(size_t size, void* data)
	{
		return malloc(size);
	}

	void UI::Free(void* ptr, void* data)
	{
		free(ptr);
	}
}
