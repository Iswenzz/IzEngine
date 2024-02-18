#include "Patch.hpp"

namespace IW3SR::Game
{
	void Patch::Initialize()
	{
		Game();
		Renderer();
		System();
	}

	void Patch::Game()
	{
		if (COD4X)
			bg_weaponNames = Signature(COD4X, 0x443DDE0);
	}

	void Patch::Renderer()
	{
		// Disable <developer 1> check for drawing debug lines & collisions
		Memory::NOP(0x6496D8, 3);

		// Increase fps cap to 125 for menus and loadscreen
		Memory::Set<char>(0x500176, 8);
		Memory::Set<char>(0x500179, 8);
	}

	void Patch::System()
	{
		if (COD4X)
			MainWndProc_h < Signature(COD4X_BIN, "55 89 E5 53 81 EC 84 00 00 00 C7 04 24 02");
	}
}
