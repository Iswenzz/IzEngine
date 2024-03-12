#include "UI.hpp"

#include "DX9/Device.hpp"
#include "Windows/Environment.hpp"
#include "Windows/Sys.hpp"

namespace IW3SR::Engine
{
	UI::UI()
	{
		ImGui::SetAllocatorFunctions(&Allocator, &Free, &Data);
		Context = ImGui::CreateContext();
		PlotContext = ImPlot::CreateContext();

		ImGui::SetCurrentContext(Context);
		ImPlot::SetImGuiContext(Context);
		ImPlot::SetCurrentContext(PlotContext);

		Memory = UC::Memory();
		Themes = UC::Themes();
	}

	UI::~UI()
	{
		ImGui::DestroyContext(Context);
		ImPlot::DestroyContext(PlotContext);
	}

	void UI::Initialize()
	{
		Environment::Deserialize("UI", *this);

		Active = true;
		ImGui_ImplWin32_Init(Sys::MainWindow);
		ImGui_ImplDX9_Init(Device::Get().D3Device);
		Themes.Initialize();
	}

	void UI::CreateScreen(const vec2& position, const vec2& size, const vec2& display)
	{
		Screen = VirtualScreen(position, size, display);
		Size = Screen.VirtualToReal.y * Scale;
	}

	void UI::Release()
	{
		Active = false;
		ImGui_ImplDX9_Shutdown();
		ImGui_ImplWin32_Shutdown();

		Environment::Serialize("UI", *this);
	}

	void UI::Begin()
	{
		if (HasBegin) return;

		HasBegin = true;
		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	void UI::End()
	{
		HasBegin = false;
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	}

	double UI::Time()
	{
		return GImGui->Time;
	}

	float UI::DeltaTime()
	{
		return ImGui::GetIO().DeltaTime;
	}

	int UI::DeltaTimeMS()
	{
		return ImGui::GetIO().DeltaTime * 1000;
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
