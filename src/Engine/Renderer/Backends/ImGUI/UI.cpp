#include "UI.hpp"

#include "Core/System/Environment.hpp"
#include "Core/System/System.hpp"
#include "DX9/Device.hpp"

namespace IzEngine
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
		IZ_ASSERT(!Active, "UI already initialized.");
		IZ_ASSERT(Environment::Initialized, "Environment not initialized.");

		Environment::Deserialize("UI", *this);
		Themes.Initialize();
		Active = true;
	}

	void UI::CreateScreen(const vec2& position, const vec2& size, const vec2& display)
	{
		Screen = VirtualScreen(position, size, display);
		Size = Screen.VirtualToReal.y * Scale;
	}

	void UI::Shutdown()
	{
		IZ_ASSERT(Active, "UI already shutdown.");
		IZ_ASSERT(Environment::Initialized, "Environment not initialized.");

		Environment::Serialize("UI", *this);
		Active = false;
	}

	void UI::Begin()
	{
		ImGui::NewFrame();

		if (Open)
		{
			Memory.Render();
			Themes.Render();
		}
	}

	void UI::End()
	{
		ImGui::Render();
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
