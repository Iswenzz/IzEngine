#pragma once
#include "Game/Definitions.hpp"
#include "Sys/Hook.hpp"
#include "Sys/Function.hpp"

static Function<dvar_s*(const char* name)> 
	Dvar_FindVar = 0x56B5D0;
static Function<void(int count, int width, GfxPointVertex* verts, bool depthTest)> 
	RB_DrawLines3D = 0x613040;
static Function<void FASTCALL(const float* colorFloat, char* colorBytes)> 
	R_ConvertColorToBytes = 0x493530;
static Function<char(Material* material, float x, float y, float w, float h, float s0,
	float t0, int s1, int t1, float* color)> 
	R_AddCmdDrawStretchPic = 0x5F65F0;
static Function<Material*(const char* fontName, int fontSize)>
	Material_RegisterHandle = 0x5F2A80;

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

EXTERN_C const char* R_AddCmdDrawText(const char* text, int maxChars, Font_s* font, float x, float y,
	float xScale, float yScale, float rotation, int style, float* color);
void ScrPlace_ApplyRect(float& x, float& y, float& w, float& h,
	RectAlignHorizontal_t horizontal, RectAlignVertical_t vertical);
