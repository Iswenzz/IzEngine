#pragma once
#include "Game/Definitions.hpp"
#include "Sys/Hook.hpp"
#include "Sys/Function.hpp"

using namespace IW3SR;

void Com_PrintMessage(int channel, const char* msg, int type);
void RB_EndSceneRendering(GfxCmdBufInput* input, GfxViewInfo* viewInfo, GfxCmdBufSourceState* src, GfxCmdBufState* buf);
IDirect3D9* STDCALL R_Direct3DCreate9(UINT sdk);
void CG_DrawCrosshair(bool isScoreboardVisible);

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

extern Hook<void(int channel, const char* msg, int type)> 
	Com_PrintMessage_h;
extern Hook<void(bool isScoreboardVisible)>
	CG_DrawCrosshair_h;
extern Hook<void(GfxCmdBufInput* input, GfxViewInfo* viewInfo, GfxCmdBufSourceState* src, GfxCmdBufState* buf)> 
	RB_EndSceneRendering_h;
extern Hook<IDirect3D9* STDCALL(UINT sdk)> 
	R_Direct3DCreate9_h;
