#include "Game.hpp"
#include <array>

Game* SR = nullptr;

namespace IW3SR
{
	Game::Game()
	{
		CoD4X();
		Environment::Load();

		Log = std::make_unique<class Log>();
		Render = std::make_unique<class Render>();
		Modules = std::make_unique<class Modules>();
		GUI = std::make_unique<class GUI>();

		Hook();
	}

	Game::~Game()
	{
		Unhook();
	}

	void Game::Initialize()
	{
		Player::Allocates();
		Modules->Initialize();
	}

	void Game::CoD4X()
	{
		if (!COD4X) return;

		uintptr_t antiHook = COD4X + 0x43580;
		uintptr_t aimAssist = 0x452BFA;
		uintptr_t localTagMatrix = 0x434200;

		Memory::Write(antiHook, "\xC3", 1);
		Memory::Write(aimAssist, "\xE8\xA1\xF9\xFA\xFF", 5);
		Memory::Write(localTagMatrix, "\x51\x53\x8B\x5C\x24", 5);
	}

	void Game::Hook()
	{
		CreateWindowExA_h.Install();
		MainWndProc_h.Install();

		Com_PrintMessage_h.Install();
		CG_DrawCrosshair_h.Install();
		R_Direct3DCreate9_h.Install();
		RB_EndSceneRendering_h.Install();
	}

	void Game::Unhook()
	{
		CreateWindowExA_h.Remove();
		MainWndProc_h.Remove();

		Com_PrintMessage_h.Remove();
		CG_DrawCrosshair_h.Remove();
		R_Direct3DCreate9_h.Remove();
		RB_EndSceneRendering_h.Remove();
	}
}
