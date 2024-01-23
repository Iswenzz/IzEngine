#include "UI.hpp"

namespace IW3SR::Engine
{
	UI::UI()
	{
		OpenKey = KeyListener(VK_F10);

		Memory = UC::Memory();
		Modules = UC::Modules();
		Settings = UC::Settings();
		Themes = UC::Themes();
	}

	void UI::Initialize()
	{
		Active = true;
		ImGui::SetAllocatorFunctions(&Allocator, &Free, &Data);
		Context = ImGui::CreateContext();
		PlotContext = ImPlot::CreateContext();
		ModuleContext();

		Themes.Initialize();
	}

	void UI::Release()
	{
		Active = false;
		ImGui::DestroyContext(Context);
		ImPlot::DestroyContext(PlotContext);
	}

	void UI::Begin()
	{
		ImGui::NewFrame();

		if (OpenKey.IsPressed())
			Open = !Open;

		Themes.ComputeRainbow();
	}

	void UI::End()
	{
		ImGui::EndFrame();
		ImGui::Render();
	}

	void UI::ModuleContext()
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
