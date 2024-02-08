#include "Functions.hpp"
#include "Game/Game.hpp"

namespace IW3SR::Game
{
	Hook<HWND STDCALL(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName,
		DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu,
		HINSTANCE hInstance, LPVOID lpParam)>
		CreateWindowExA_h(CreateWindowExA, Sys::CreateMainWindow);

	Hook<LRESULT CALLBACK(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)>
		MainWndProc_h(0x57BB20, Sys::MainWndProc);

	Hook<void(int channel, const char* msg, int type)>
		Com_PrintMessage_h(0x4FCA50, Console::Write);

	Hook<void(int localClientNum)>
		CG_DrawCrosshair_h(0x42F6B5, Renderer::Draw2D);

	Hook<void(usercmd_s* cmd)>
		CL_FinishMove_h(0x463A60, PMove::FinishMove);

	Hook<void(pmove_t* pm, pml_t* pml)>
		PM_WalkMove_h(0x40F7A0, PMove::WalkMove);

	Hook<void(pmove_t* pm, pml_t* pml)>
		PM_AirMove_h(0x40F680, PMove::AirMove);

	Hook<void()>
		R_Init_h(0x5F4EE0, Renderer::Initialize);

	Hook<void(int window)>
		R_Shutdown_h(0x5F4F90, Renderer::Shutdown);

	Hook<IDirect3D9* STDCALL(UINT sdk)>
		R_Direct3DCreate9_h(0x670284, D3D9::Direct3DCreate9);

	Hook<void(GfxCmdBufInput* cmd, GfxViewInfo* viewInfo, GfxCmdBufSourceState* src, GfxCmdBufState* buf)>
		RB_EndSceneRendering_h(0x6496EC, Renderer::Draw3D);
}
