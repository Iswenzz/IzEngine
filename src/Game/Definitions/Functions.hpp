#pragma once
#include "Sys/Memory/Function.hpp"
#include "Sys/Memory/Assembler.hpp"

namespace IW3SR::Game
{
	API extern Function<void(const trajectory_t* tr, int atTime, float* out)>
		BG_EvaluateTrajectory;

	API extern Function<int(const char* name)>
		BG_FindWeaponIndexByName;

	API extern Function<void(trace_t* result, const vec3_t start, const vec3_t mins, const vec3_t maxs,
		const vec3_t end, int skipEntity, int tracemask)>
		CG_Trace;

	API extern Function<void(int localClientNum, int controllerIndex, const char* text)>
		Cmd_ExecuteSingleCommand;

	API extern Function<dvar_s*(const char* name)>
		Dvar_FindVar;

	extern Function<Material*(const char* material, int size)>
		Material_RegisterHandle;

	API extern Function<void(pmove_t* pm, pml_t* pml)>
		PM_GroundTrace;

	extern Function<void(const char* text, int maxChars, Font_s* font, float x, float y,
		float xScale, float yScale, float rotation, int style, float* color)>
		R_AddCmdDrawText;

	extern Function<void(Material* material, float x, float y, float w, float h,
		float null1, float null2, float null3, float null4, float* color)>
		R_AddCmdDrawStretchPic;

	extern Function<void FASTCALL(const float* colorFloat, char* colorBytes)>
		R_ConvertColorToBytes;

	extern Function<Font_s*(const char* font, int size)>
		R_RegisterFont;

	extern Function<void(GfxCmdBufSourceState* source, float gameTime)>
		R_SetGameTime;

	extern Function<int(const char* text, int maxChars, Font_s* font)>
		R_TextWidth;

	extern Function<void(int count, int width, GfxPointVertex* verts, bool depthTest)>
		RB_DrawLines3D;
}
namespace IW3SR::Game
{
	ASM_FUNCTION(BG_EvaluateTrajectory);
	ASM_FUNCTION(Dvar_FindVar);
	ASM_FUNCTION(R_AddCmdDrawText);
	ASM_FUNCTION(R_AddCmdDrawStretchPic);
	ASM_FUNCTION(R_SetGameTime);
	ASM_FUNCTION(R_TextWidth);
}
