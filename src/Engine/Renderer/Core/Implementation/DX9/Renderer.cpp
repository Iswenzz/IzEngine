#include "Renderer/Common.hpp"

#include "Core/Input/Keyboard.hpp"
#include "Core/System/Plugins.hpp"
#include "Core/System/System.hpp"

#include "DX9/Device.hpp"
#include "ImGUI/UI.hpp"

namespace IzEngine
{
	void Renderer::Initialize()
	{
		IZ_ASSERT(OSWindow::Handle, "Renderer needs a main window.");
		IZ_ASSERT(Device::D3Device, "D3Device is not initialized.");

		Textures::Initialize();
		Fonts::Initialize();

		ImGui_ImplOS_Init(OSWindow::Handle);
		ImGui_ImplDX9_Init(Device::D3Device);
		UI::Get().Initialize();

		EventPluginInitialize event;
		Plugins::Dispatch(event);
	}

	void Renderer::Shutdown()
	{
		ImGui_ImplOS_Shutdown();
		ImGui_ImplDX9_Shutdown();
		UI::Get().Shutdown();

		Fonts::Release();
		Textures::Release();
	}

	void Renderer::Begin()
	{
		ImGui_ImplOS_NewFrame();
		ImGui_ImplDX9_NewFrame();
		UI::Get().Begin();
	}

	void Renderer::End()
	{
		UI::Get().End();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		Keyboard::Reset();
	}

	void Renderer::Frame()
	{
		Device::D3Device->Clear(0, nullptr, D3DCLEAR_TARGET, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
		Device::D3Device->BeginScene();
		Begin();

		EventRendererRender event;
		Application::Get().Dispatch(event);

		End();
		Device::D3Device->EndScene();
		Device::D3Device->Present(nullptr, nullptr, nullptr, nullptr);
	}
}
