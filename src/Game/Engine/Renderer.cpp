#include "Renderer.hpp"
#include "Assets.hpp"
#include "Drawing/Draw2D.hpp"
#include "Drawing/Draw3D.hpp"

#include "Engine/Backends/DX9/Assets.hpp"

namespace IW3SR::Game
{
	Renderer::Renderer()
	{
		Patch();
	}

	void Renderer::Patch()
	{
		// Disable <developer 1> check for drawing debug lines & collisions
		Memory::NOP(0x6496D8, 3);

		// Increase fps cap to 125 for menus and loadscreen
		Memory::Set<char>(0x500176, 8);
		Memory::Set<char>(0x500179, 8);
	}

	void Renderer::Initialize()
	{
		R_Init_h();

		Assets::Get().Initialize();
		Engine::Assets::Get().Initialize();

		GUI::Get().Initialize();
		Modules::Get().Initialize();
		Features::Get().Initialize();
	}

	void Renderer::Shutdown(int window)
	{
		Features::Get().Release();
		Modules::Get().Release();
		GUI::Get().Release();

		Assets::Get().Release();
		Engine::Assets::Get().Release();

		R_Shutdown_h(window);
	}

	void Renderer::Draw3D(GfxCmdBufInput* cmd, GfxViewInfo* viewInfo, GfxCmdBufSourceState* src, GfxCmdBufState* buf)
	{
		GameCallback(OnDraw3D);
		Game::Draw3D::Render();
		RB_EndSceneRendering_h(cmd, viewInfo, src, buf);
	}

	void Renderer::Draw2D(int localClientNum)
	{
		GameCallback(OnDraw2D);
		CG_DrawCrosshair_h(localClientNum);
	}

	void Renderer::Render()
	{
		GUI::Get().Begin();
		if (Player::CanRender())
			GameCallback(OnRender);
		GUI::Get().End();
	}
}
