#include "Renderer.hpp"
#include "Assets.hpp"
#include "Drawing/Draw2D.hpp"
#include "Drawing/Draw3D.hpp"
#include "Features/Features.hpp"
#include "Modules/Modules.hpp"

#include "Engine/Backends/DX9/Device.hpp"

namespace IW3SR::Game
{
	void Renderer::Initialize()
	{
		R_Init_h();

		Device::Get().Assign(dx->d3d9, dx->device);
		Device::Get().CreateScreen();
		Assets::Get().Initialize();
		GUI::Get().Initialize();

		Plugins::Initialize();
		Modules::Get().Initialize();
		Features::Get().Initialize();
	}

	void Renderer::Shutdown(int window)
	{
		Features::Get().Release();
		Modules::Get().Release();
		Plugins::Shutdown();

		GUI::Get().Release();
		Assets::Get().Release();

		R_Shutdown_h(window);
	}

	void Renderer::Draw2D(int localClientNum)
	{
		CG_DrawCrosshair_h(localClientNum);
		GameCallback(OnDraw2D);
	}

	void Renderer::Draw3D(GfxCmdBufInput* cmd, GfxViewInfo* viewInfo, GfxCmdBufSourceState* src, GfxCmdBufState* buf)
	{
		RB_EndSceneRendering_h(cmd, viewInfo, src, buf);

		Draw3D::Render();
		GameCallback(OnDraw3D);
		GameCallback(OnDraw3D, cmd, viewInfo, src, buf);
	}

	void Renderer::Render()
	{
		UI::Get().Begin();
		GUI::Get().Render();

		if (Player::CanRender())
			GameCallback(OnRender);

		UI::Get().End();
		KeyListener::Reset();
	}
}
