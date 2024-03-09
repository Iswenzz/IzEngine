#include "Game.hpp"

namespace IW3SR::Game
{
	void Application::Start()
	{
		Environment::Initialize();
		Environment::Load();

		CoD4X();

		Patch::Get().Initialize();
		Console::Get().Initialize();
		Player::Allocates();
		Plugins::Initialize();

		Hook();
	}

	void Application::Shutdown()
	{
		Environment::Save();
		Console::Get().Release();
		Plugins::Shutdown();

		Unhook();
	}

	void Application::CoD4X()
	{
		const auto cod4x = std::ranges::find_if(Environment::Modules,
			[](const auto& m) { return m.find("cod4x_") != std::string::npos; });

		if (cod4x == Environment::Modules.end())
			return;

		COD4X_BIN = *cod4x;
		COD4X_HANDLE = Signature(GetModuleHandle(COD4X_BIN.c_str()));
	}

	void Application::Hook()
	{
		CreateWindowExA_h.Install();
		Direct3DCreate9_h.Install();
		MainWndProc_h.Install();

		Cmd_ExecuteSingleCommand_h.Install();
		Com_PrintMessage_h.Install();
		CG_DrawCrosshair_h.Install();
		CG_PredictPlayerState_Internal_h.Install();
		CL_FinishMove_h.Install();
		PM_WalkMove_h.Install();
		PM_AirMove_h.Install();
		R_Init_h.Install();
		R_RenderAllLeftovers_h.Install();
		R_Shutdown_h.Install();
		RB_EndSceneRendering_h.Install();
	}

	void Application::Unhook()
	{
		CreateWindowExA_h.Remove();
		Direct3DCreate9_h.Remove();
		MainWndProc_h.Remove();

		Cmd_ExecuteSingleCommand_h.Remove();
		Com_PrintMessage_h.Remove();
		CG_DrawCrosshair_h.Remove();
		CG_PredictPlayerState_Internal_h.Remove();
		CL_FinishMove_h.Remove();
		PM_WalkMove_h.Remove();
		PM_AirMove_h.Remove();
		R_Init_h.Remove();
		R_RenderAllLeftovers_h.Remove();
		R_Shutdown_h.Remove();
		RB_EndSceneRendering_h.Remove();
	}

	void Application::Dispatch(Event& event)
	{
		Plugins::Dispatch(event);

		Modules::Get().Dispatch(event);
		Settings::Get().Dispatch(event);
	}
}
