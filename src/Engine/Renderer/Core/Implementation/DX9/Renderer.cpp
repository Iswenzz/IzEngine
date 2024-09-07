#include "Renderer/Common.hpp"

#include "DX9/Device.hpp"
#include "ImGUI/UI.hpp"

#include "Core/Communication/Actions.hpp"
#include "Core/Input/Keyboard.hpp"
#include "Core/System/Plugins.hpp"
#include "Core/System/Window.hpp"

namespace IzEngine
{
	void Renderer::Initialize()
	{
		IZ_ASSERT(OSWindow::Handle, "Window is not initialized.");
		IZ_ASSERT(Device::D3Device, "Device is not initialized.");

		Textures::Initialize();
		Fonts::Initialize();
		UI::Initialize();

		ImGui_ImplOS_Init(OSWindow::Handle);
		ImGui_ImplAPI_Init(Device::D3Device);

		Plugins::Initialize();
	}

	void Renderer::Shutdown()
	{
		Plugins::Shutdown();

		ImGui_ImplAPI_Shutdown();
		ImGui_ImplOS_Shutdown();

		UI::Shutdown();
		Fonts::Release();
		Textures::Release();
	}

	void Renderer::Resize(const vec2& size)
	{
		Device::Resize(size);
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

		Keyboard::Reset();
	}

	void Renderer::Frame()
	{
		Begin();

		EventRendererRender event;
		Application::Dispatch(event);

		End();
	}
}
