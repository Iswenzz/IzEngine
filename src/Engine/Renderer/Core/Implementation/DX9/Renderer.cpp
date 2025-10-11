#include "Renderer/Common.hpp"

#include "DX9/Device.hpp"
#include "ImGUI/UI.hpp"

#include "Core/Communication/Actions.hpp"
#include "Core/System/Plugins.hpp"
#include "Core/System/Window.hpp"

namespace IzEngine
{
	void Renderer::Initialize()
	{
		IZ_ASSERT(!Active, "Renderer already initialized.");
		IZ_ASSERT(Window::Handle, "Window is not initialized.");
		IZ_ASSERT(Device::D3Device, "Device is not initialized.");

		InitializeAssets();

		UI::Initialize();
		ImGui_ImplOS_Init(Window::Handle);
		ImGui_ImplAPI_Init(Device::D3Device);

		Active = true;
	}

	void Renderer::InitializeAssets()
	{
		Textures::Initialize();
		Fonts::Initialize();
		Plugins::Initialize();
	}

	void Renderer::Shutdown()
	{
		IZ_ASSERT(Active, "Renderer already shutdown.");

		ImGui_ImplAPI_Shutdown();
		ImGui_ImplOS_Shutdown();
		UI::Shutdown();

		ShutdownAssets();

		Active = false;
	}

	void Renderer::ShutdownAssets()
	{
		Textures::Shutdown();
		Fonts::Shutdown();
		Plugins::Shutdown();
	}

	void Renderer::Resize(const vec2& size)
	{
		Device::Resize(size);
		UI::Resize(size);
	}

	void Renderer::Begin()
	{
		Device::Begin();
		ImGui_ImplOS_NewFrame();
		ImGui_ImplAPI_NewFrame();
		UI::Begin();

		Actions::Submit();
	}

	void Renderer::End()
	{
		UI::End();
		ImGui_ImplAPI_RenderDrawData(ImGui::GetDrawData());
		Device::End();

		Input::Reset();
	}

	void Renderer::Frame()
	{
		if (!Active)
			return;

		Begin();

		EventRendererRender event;
		Application::Dispatch(event);

		End();
	}
}
