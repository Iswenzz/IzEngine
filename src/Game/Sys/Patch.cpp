#include "Patch.hpp"

namespace IW3SR::Game
{
	void Patch::Initialize()
	{
		Renderer();
		CoD4X();
		System();
	}

	void Patch::Renderer()
	{
		// Disable <developer 1> check for drawing debug lines & collisions
		Memory::NOP(0x6496D8, 3);

		// Increase fps cap to 125 for menus and loadscreen
		Memory::Set<char>(0x500176, 8);
		Memory::Set<char>(0x500179, 8);
	}

	void Patch::CoD4X()
	{
		bg_weaponNames = reinterpret_cast<WeaponDef**>(COD4X + 0x443DDE0);
	}

	void Patch::System()
	{
		if (COD4X)
		{
			MainWndProc_h.Address = Memory::Scan(COD4X_BIN,
				"\x55\x89\xE5\x53\x81\xEC\x84\x00\x00\x00\xC7\x04\x24\x02", 14);
		}
	}
}
