#include "Render.hpp"
#include "Game/Game.hpp"
#include "Sys/Memory.hpp"
#include "Utils/Math.hpp"

namespace IW3SR
{
	Render::Render()
	{
		// Disable <developer 1> check for drawing debug lines & collisions
		Memory::NOP(0x6496D8, 3);

		RB_EndSceneRendering_h.Install();
		R_Direct3DCreate9_h.Install();
	}

	void Render::Frame() { }
}
