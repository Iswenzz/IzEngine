#include "Functions.hpp"
#include "Game/Game.hpp"

Hook<void(int channel, const char* msg, int type)> 
	Com_PrintMessage_h(0x4FCA50, Com_PrintMessage);
Hook<void(bool isScoreboardVisible)>
	CG_DrawCrosshair_h(0x42F6B5, CG_DrawCrosshair);
Hook<void(GfxCmdBufInput* input, GfxViewInfo* viewInfo, GfxCmdBufSourceState* src, GfxCmdBufState* buf)> 
	RB_EndSceneRendering_h(0x6496EC, RB_EndSceneRendering);
Hook<IDirect3D9* STDCALL(UINT sdk)>
	R_Direct3DCreate9_h(0x670284, R_Direct3DCreate9);

void Com_PrintMessage(int channel, const char* msg, int type)
{
	Log::Write(channel, type, msg);
	Com_PrintMessage_h(channel, msg, type);
}

void CG_DrawCrosshair(bool isScoreboardVisible)
{
	SR->Render->Draw(isScoreboardVisible);
	CG_DrawCrosshair_h(isScoreboardVisible);
}

void RB_EndSceneRendering(GfxCmdBufInput* input, GfxViewInfo* viewInfo, GfxCmdBufSourceState* src, GfxCmdBufState* buf)
{
	SR->Render->Frame();
	RB_EndSceneRendering_h(input, viewInfo, src, buf);
}

IDirect3D9* STDCALL R_Direct3DCreate9(UINT sdk)
{
	IDirect3D9Ex* d3d9ex_device = nullptr;
	Log::WriteLine("Getting Direct3D 9 EX interface...");

	if (SUCCEEDED(Direct3DCreate9Ex(sdk, &d3d9ex_device)))
		return (new D3D9EX(d3d9ex_device));

	Log::WriteLine("Direct3D 9 EX failed to initialize. Defaulting to Direct3D 9.");
	return (new D3D9(Direct3DCreate9(sdk)));
}
