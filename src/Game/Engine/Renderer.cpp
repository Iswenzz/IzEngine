#include "Renderer.hpp"
#include "Assets.hpp"
#include "Drawing/Draw2D.hpp"
#include "Drawing/Draw3D.hpp"

#include "Engine/Backends/DX9/Assets.hpp"

namespace IW3SR
{
	Renderer::Renderer()
	{
		Modules = std::make_unique<class Modules>();
		Features = std::make_unique<class Features>();
		GUI = std::make_unique<class GUI>();

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

		Assets::Initialize();
		Game::Assets::Initialize();

		GetGUI()->Initialize();
		GetRenderer()->Modules->Initialize();
		GetRenderer()->Features->Initialize();
	}

	void Renderer::Shutdown(int window)
	{
		GetRenderer()->Features->Shutdown();
		GetRenderer()->Modules->Shutdown();
		GetGUI()->Shutdown();

		Game::Assets::Shutdown();
		Assets::Shutdown();

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
		GUI->Begin();
		if (Player::CanRender())
			GameCallback(OnRender);
		GUI->End();
	}
}
