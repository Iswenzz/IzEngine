#include "Render.hpp"
#include "Game/Game.hpp"
#include "Game/Render/Draw.hpp"
#include "Sys/Memory.hpp"
#include "Utils/Math.hpp"

namespace IW3SR
{
	Render::Render()
	{
		// Disable <developer 1> check for drawing debug lines & collisions
		Memory::NOP(0x6496D8, 3);

		// Increase fps cap to 125 for menus and loadscreen
		Memory::Set<uint8_t>(0x500176, 8);
		Memory::Set<uint8_t>(0x500179, 8);

		RB_EndSceneRendering_h.Install();
		R_Direct3DCreate9_h.Install();
	}

	void Render::Frame() 
	{
		Draw::Frame();
		ModuleCallback(OnFrame);
	}
}
