#pragma once
#include "Engine/Sys/Memory.hpp"

namespace IW3SR::Game
{
	static Function<int(const char* name)>
		BG_FindWeaponIndexByName = 0x416610;

	static Function<void(trace_t* result, const vec3_t start, const vec3_t mins, const vec3_t maxs,
		const vec3_t end, int skipEntity, int tracemask)>
		CG_Trace = 0x45A230;

	static Function<void(int localClientNum, int controllerIndex, const char* text)>
		Cmd_ExecuteSingleCommand = 0x4F9AB0;

	static Function<Material* (const char* material, int size)>
		Material_RegisterHandle = 0x5F2A80;

	static Function<void(pmove_t* pm, pml_t* pml)>
		PM_GroundTrace = 0x410660;

	static Function<void FASTCALL(const float* colorFloat, char* colorBytes)>
		R_ConvertColorToBytes = 0x493530;

	static Function<Font_s* (const char* font, int size)>
		R_RegisterFont = 0x5F1EC0;

	static Function<void(int count, int width, GfxPointVertex* verts, bool depthTest)>
		RB_DrawLines3D = 0x613040;
}
ASM
{
	using namespace IW3SR::Game;

	void BG_EvaluateTrajectory(const trajectory_t * tr, int atTime, float* out);

	void CG_TracePoint(pmove_t* pm, trace_t* trace, const float* start,
		const float* mins, const float* maxs, const float* end, int passEntityNum, int contentMask);

	dvar_s* Dvar_FindVar(const char* name);

	void R_AddCmdDrawText(const char* text, int maxChars, Font_s* font, float x, float y,
		float xScale, float yScale, float rotation, int style, float* color);

	void R_AddCmdDrawStretchPic(Material* material, float x, float y, float w, float h,
		float null1, float null2, float null3, float null4, float* color);

	int R_TextWidth(const char* text, int maxChars, Font_s* font);
}
