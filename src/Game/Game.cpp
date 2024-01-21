#include "Game.hpp"

namespace IW3SR::Game
{
	GameClient::GameClient()
	{
		Environment::Initialize();
		Environment::Load();

		CoD4X();

		Renderer = std::make_unique<class Renderer>();
		Console = std::make_unique<class Console>();

		Hook();
	}

	GameClient::~GameClient()
	{
		Environment::Save();
		Plugins::Shutdown();

		Console.reset();
		Renderer.reset();

		Unhook();
	}

	void GameClient::Start()
	{
		Player::Allocates();
		Plugins::Initialize();
	}

	void GameClient::CoD4X()
	{
		auto cod4x = std::ranges::find_if(Environment::Modules,
			[](const auto& m) { return m.find("cod4x_") != std::string::npos; });

		if (cod4x == Environment::Modules.end())
			return;

		COD4X_BIN = *cod4x;
		COD4X = reinterpret_cast<uintptr_t>(GetModuleHandle(COD4X_BIN.c_str()));

		uintptr_t antiHook = COD4X + 0x43580;
		uintptr_t aimAssist = 0x452BFA;
		uintptr_t localTagMatrix = 0x434200;

		Memory::Write(antiHook, "\xC3", 1);
		Memory::Write(aimAssist, "\xE8\xA1\xF9\xFA\xFF", 5);
		Memory::Write(localTagMatrix, "\x51\x53\x8B\x5C\x24", 5);

		bg_weaponNames = reinterpret_cast<WeaponDef**>(COD4X + 0x443DDE0);
	}

	void GameClient::Hook()
	{
		CreateWindowExA_h.Install();
		MainWndProc_h.Install();

		Com_PrintMessage_h.Install();
		CG_DrawCrosshair_h.Install();
		CL_FinishMove_h.Install();
		R_Init_h.Install();
		R_Shutdown_h.Install();
		R_Direct3DCreate9_h.Install();
		RB_EndSceneRendering_h.Install();
	}

	void GameClient::Unhook()
	{
		CreateWindowExA_h.Remove();
		MainWndProc_h.Remove();

		Com_PrintMessage_h.Remove();
		CG_DrawCrosshair_h.Remove();
		CL_FinishMove_h.Remove();
		R_Init_h.Remove();
		R_Shutdown_h.Remove();
		R_Direct3DCreate9_h.Remove();
		RB_EndSceneRendering_h.Remove();
	}
}

GameClient* GC = nullptr;
