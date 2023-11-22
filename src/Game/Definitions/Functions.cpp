#include "Functions.hpp"
#include "Game/Game.hpp"

namespace IW3SR
{
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

	Hook<void(usercmd_s* cmd)>
		CL_FinishMove_h(0x463A60, PMove::FinishMove);

	Hook<IDirect3D9* STDCALL(UINT sdk)>
		R_Direct3DCreate9_h(0x670284, D3D9::Direct3DCreate9);

	Hook<void(GfxCmdBufInput* cmd, GfxViewInfo* viewInfo, GfxCmdBufSourceState* src, GfxCmdBufState* buf)>
		RB_EndSceneRendering_h(0x6496EC, Render::Draw3D);
}

void CG_TracePoint(pmove_t* pm, trace_t* results, const float* start,
	const float* mins, const float* maxs, const float* end, int passEntityNum, int contentMask)
{
	const static uint32_t address = 0x40E160;
	__asm
	{
		push	contentMask;
		push	passEntityNum;
		push	[end];
		push	[maxs];
		push	[mins];
		push	[start];
		push	results;
		mov		esi, pm;
		call	address;
		add     esp, 1Ch;
	}
}
