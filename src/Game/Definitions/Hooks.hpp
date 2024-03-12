#pragma once
#include "Sys/Memory/Hook.hpp"
#include "Sys/Memory/Assembler.hpp"

namespace IW3SR::Game
{
	extern Hook<HWND STDCALL(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName,
		DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu,
		HINSTANCE hInstance, LPVOID lpParam)>
		CreateWindowExA_h;

	extern Hook<IDirect3D9* STDCALL(UINT sdk)>
		Direct3DCreate9_h;

	extern Hook<LRESULT CALLBACK(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)>
		MainWndProc_h;

	extern Hook<void(int localClientNum, int controllerIndex, char* command)>
		Cmd_ExecuteSingleCommand_h;

	extern Hook<void(ConChannel channel, const char* msg, int type)>
		Com_PrintMessage_h;

	extern Hook<void(int localClientNum)>
		CG_PredictPlayerState_Internal_h;

	extern Hook<void(int localClientNum)>
		CG_DrawCrosshair_h;

	extern Hook<void(usercmd_s* cmd)>
		CL_FinishMove_h;

	extern Hook<void(pmove_t* pm, pml_t* pml)>
		PM_WalkMove_h;

	extern Hook<void(pmove_t* pm, pml_t* pml)>
		PM_AirMove_h;

	extern Hook<void()>
		R_Init_h;

	extern Hook<void(void* cmds)>
		R_RenderAllLeftovers_h;

	extern Hook<void(int window)>
		R_Shutdown_h;

	extern Hook<void(GfxCmdBufInput* cmd, GfxViewInfo* viewInfo, GfxCmdBufSourceState* src, GfxCmdBufState* buf)>
		RB_EndSceneRendering_h;
}
namespace IW3SR::Game
{
	ASM_FUNCTION(R_RenderAllLeftovers_h);
}
