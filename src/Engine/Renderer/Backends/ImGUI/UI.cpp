#include "UI.hpp"

#include "Drawing/Notification.hpp"
#include "UI/Memory.hpp"
#include "UI/Themes.hpp"

#include "Core/System/Environment.hpp"

namespace IzEngine
{
	void UI::Initialize()
	{
		IZ_ASSERT(!Active, "UI already initialized.");

		Context = ImGui::CreateContext();
		PlotContext = ImPlot::CreateContext();
		InitializeContext();

		Environment::Load(Serialized, "ui.json");

		KeyOpen = Keyboard(Key_F10);
		if (Serialized.contains("KeyOpen"))
			KeyOpen = Serialized["KeyOpen"];

		CreateWindow<UC::Themes>();
		CreateWindow<UC::Memory>();

		Active = true;
	}

	void UI::InitializeContext()
	{
		ImGui::SetAllocatorFunctions(Allocator, Free, &Data);
		ImGui::SetCurrentContext(Context);
		ImPlot::SetImGuiContext(Context);
		ImPlot::SetCurrentContext(PlotContext);
	}

	void UI::Shutdown()
	{
		IZ_ASSERT(Active, "UI already shutdown.");

		ImGui::DestroyContext(Context);
		ImPlot::DestroyContext(PlotContext);

		for (const auto& [_, window] : Windows)
		{
			window->Serialize(Serialized[window->Name]);
			window->Release();
		}
		Serialized["KeyOpen"] = KeyOpen;
		Environment::Save(Serialized, "ui.json");
		Windows.clear();

		Active = false;
	}

	void UI::CreateScreen(const vec2& position, const vec2& size, const vec2& display)
	{
		Screen = VirtualScreen(position, size, display);
		Size = Screen.VirtualToReal.y * Scale;
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

	void* UI::Allocator(size_t size, void* data)
	{
		return malloc(size);
	}

	void UI::Free(void* ptr, void* data)
	{
		free(ptr);
	}

	void UI::Begin()
	{
		ImGui::NewFrame();
		UC::Themes::ComputeRainbow();
		Notifications::Render();

		if (Keyboard::IsPressed(Key_Escape))
			Open = false;

		if (KeyOpen.IsPressed())
			Open = !Open;
	}

	void UI::End()
	{
		ImGui::Render();
	}

	void UI::Dispatch(Event& event)
	{
		for (const auto& [_, window] : Windows)
			window->OnEvent(event);
	}
}
