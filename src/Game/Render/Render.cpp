#include "Render.hpp"
#include "Game/Game.hpp"
#include "Game/Render/Draw.hpp"
#include "Sys/Memory.hpp"

namespace IW3SR
{
	Render::Render()
	{
		// Disable <developer 1> check for drawing debug lines & collisions
		Memory::NOP(0x6496D8, 3);

		// Increase fps cap to 125 for menus and loadscreen
		Memory::Set<uint8_t>(0x500176, 8);
		Memory::Set<uint8_t>(0x500179, 8);

		CG_DrawCrosshair_h.Install();
		RB_EndSceneRendering_h.Install();
		R_Direct3DCreate9_h.Install();
	}

	void Render::Draw3D(GfxCmdBufInput* input, GfxViewInfo* viewInfo, GfxCmdBufSourceState* src, GfxCmdBufState* buf)
	{
		Draw3D::Frame();
		ModuleCallback(OnDraw3D);
		RB_EndSceneRendering_h(input, viewInfo, src, buf);
	}

	void Render::Draw2D(bool scoreboard)
	{
		ModuleCallback(OnDraw2D);
		CG_DrawCrosshair_h(scoreboard);
	}

	void Render::Frame()
	{
		GUI::Begin();
		ModuleCallback(OnFrame);
		GUI::End();
	}
}
