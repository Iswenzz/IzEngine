#include "Patch.hpp"

#define COD4X(s) if (COD4X_HANDLE) s;

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
		COD4X(bg_weaponNames = Signature(COD4X_HANDLE, 0x443DDE0));
	}

	void Patch::Renderer()
	{
		Sleep(2000);

		// Disable <developer 1> check for debug rendering
		Memory::NOP(0x6496D8, 3);

		// Increase fps cap for menus and loadscreen
		Memory::NOP(0x5001A8, 2);
		COD4X(Memory::NOP(Signature(COD4X_BIN, "72 ?? 83 ?? 00 F9 C5 00 07"), 2));
	}

	void Patch::System()
	{
		COD4X(MainWndProc_h < Signature(COD4X_BIN, "55 89 E5 53 81 EC 84 00 00 00 C7 04 24 02"));
	}
}
