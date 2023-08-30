#include "Game.hpp"
#include "Sys/Memory.hpp"

#include <MinHook.h>

namespace IW3SR
{
	Game::Game()
	{
		MH_Initialize();

		Log = std::make_unique<class Log>();
		Modules = std::make_unique<class Modules>();
	}

	Game::~Game()
	{
		MH_Uninitialize();
	}

	void Game::Patch()
	{
		if (!COD4X) return;

		uint32_t antiHook = COD4X + 0x43580;
		uint32_t aimAssist = 0x452BFA;
		uint32_t localTagMatrix = 0x434200;

		Memory::Write(antiHook, "\xC3", 1);
		Memory::Write(aimAssist, "\xE8\xA1\xF9\xFA\xFF", 5);
		Memory::Write(localTagMatrix, "\x51\x53\x8B\x5C\x24", 5);
	}

	void Game::Frame()
	{

	}
}
