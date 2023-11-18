#include "Functions.hpp"
#include "Game/Game.hpp"
#include "Game/Player/PMove.hpp"

Hook<HWND STDCALL(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName,
	DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu,
	HINSTANCE hInstance, LPVOID lpParam)>
	CreateWindowExA_h(CreateWindowExA, GUI::CreateMainWindow);

Hook<LRESULT CALLBACK(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)>
	MainWndProc_h(0x57BB20, GUI::MainWndProc);

Hook<void(int channel, const char* msg, int type)> 
	Com_PrintMessage_h(0x4FCA50, Log::Write);

Hook<void(bool scoreboard)>
	CG_DrawCrosshair_h(0x42F6B5, Render::Draw2D);

Hook<void(GfxCmdBufInput* input, GfxViewInfo* viewInfo, GfxCmdBufSourceState* src, GfxCmdBufState* buf)> 
	RB_EndSceneRendering_h(0x6496EC, Render::Draw3D);

Hook<IDirect3D9* STDCALL(UINT sdk)> 
	R_Direct3DCreate9_h(0x670284, D3D9::Direct3DCreate9);

Hook<void(usercmd_s* cmd)>
    CL_FinishMove_h(0x463A60, PMove::FinishMove);
