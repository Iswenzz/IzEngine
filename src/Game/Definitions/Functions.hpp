#pragma once
#include "Game/Definitions.hpp"
#include "Sys/Hook.hpp"
#include "Sys/Function.hpp"
#include "Sys/Memory.hpp"

static Function<void(int count, int width, GfxPointVertex* verts, bool depthTest)> 
	RB_DrawLines3D = 0x613040;
static Function<void FASTCALL(const float* colorFloat, char* colorBytes)> 
	R_ConvertColorToBytes = 0x493530;
static Function<Material*(const char* material, int size)>
	Material_RegisterHandle = 0x5F2A80;
static Function<Font_s*(const char* font, int size)>
	R_RegisterFont = 0x5F1EC0;

extern Hook<HWND STDCALL(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName,
	DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu,
	HINSTANCE hInstance, LPVOID lpParam)>
	CreateWindowExA_h;
extern Hook<LRESULT CALLBACK(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)>
	MainWndProc_h;
extern Hook<void(int channel, const char* msg, int type)> 
	Com_PrintMessage_h;
extern Hook<void(bool scoreboard)>
	CG_DrawCrosshair_h;
extern Hook<void(GfxCmdBufInput* input, GfxViewInfo* viewInfo, GfxCmdBufSourceState* src, GfxCmdBufState* buf)> 
	RB_EndSceneRendering_h;
extern Hook<IDirect3D9* STDCALL(UINT sdk)> 
	R_Direct3DCreate9_h;

EXTERN_C
{
	dvar_s* Dvar_FindVar(const char* name);
	void ScrPlace_ApplyRect(float& x, float& y, float& w, float& h,
		RectAlignHorizontal_t horizontal, RectAlignVertical_t vertical);
	void R_AddCmdDrawText(const char* text, int maxChars, Font_s* font, float x, float y,
		float xScale, float yScale, float rotation, int style, float* color);
	void R_AddCmdDrawStretchPic(Material* material, float x, float y, float w, float h, 
		float null1, float null2, float null3, float null4, float* color);
	int R_TextWidth(const char* text, int maxChars, Font_s* font);
}
