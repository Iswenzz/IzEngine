#include "Functions.hpp"
using namespace asmjit;

namespace IW3SR::Game
{
	Function<void(const trajectory_t* tr, int atTime, float* out)>
		BG_EvaluateTrajectory = ASM_LOAD_FUNCTION(BG_EvaluateTrajectory);

	Function<int(const char* name)>
		BG_FindWeaponIndexByName = 0x416610;

	Function<void(trace_t* result, const vec3_t start, const vec3_t mins, const vec3_t maxs,
		const vec3_t end, int skipEntity, int tracemask)>
		CG_Trace = 0x45A230;

	Function<void(int localClientNum, int controllerIndex, const char* text)>
		Cmd_ExecuteSingleCommand = 0x4F9AB0;

	Function<dvar_s* (const char* name)>
		Dvar_FindVar = ASM_LOAD_FUNCTION(Dvar_FindVar);

	Function<Material* (const char* material, int size)>
		Material_RegisterHandle = 0x5F2A80;

	Function<void(pmove_t* pm, pml_t* pml)>
		PM_GroundTrace = 0x410660;

	Function<void(const char* text, int maxChars, Font_s* font, float x, float y,
		float xScale, float yScale, float rotation, int style, float* color)>
		R_AddCmdDrawText = ASM_LOAD_FUNCTION(R_AddCmdDrawText);

	Function<void(Material* material, float x, float y, float w, float h,
		float null1, float null2, float null3, float null4, float* color)>
		R_AddCmdDrawStretchPic = ASM_LOAD_FUNCTION(R_AddCmdDrawStretchPic);

	Function<void FASTCALL(const float* colorFloat, char* colorBytes)>
		R_ConvertColorToBytes = 0x493530;

	Function<Font_s* (const char* font, int size)>
		R_RegisterFont = 0x5F1EC0;

	Function<int(const char* text, int maxChars, Font_s* font)>
		R_TextWidth = ASM_LOAD_FUNCTION(R_TextWidth);

	Function<void(int count, int width, GfxPointVertex* verts, bool depthTest)>
		RB_DrawLines3D = 0x613040;
}
namespace IW3SR::Game
{
	ASM_FUNCTION(BG_EvaluateTrajectory)
	{
		a.push(x86::ebp);
		a.mov(x86::ebp, x86::esp);
		a.pushad();

		a.mov(x86::ecx, x86::dword_ptr(x86::ebp, 0x10));	// out
		a.mov(x86::eax, x86::dword_ptr(x86::ebp, 0x0C));	// atTime
		a.push(x86::dword_ptr(x86::ebp, 0x08));				// tr
		a.call(0x40BD70);
		a.add(x86::esp, 4);

		a.popad();
		a.pop(x86::ebp);
		a.ret();
	}

	ASM_FUNCTION(Dvar_FindVar)
	{
		a.push(x86::ebp);
		a.mov(x86::ebp, x86::esp);
		a.pushad();

		a.mov(x86::edi, x86::dword_ptr(x86::ebp, 0x8));		// name
		a.call(0x56B5D0);
		a.mov(x86::dword_ptr(x86::esp, 0x1C), x86::eax);

		a.popad();
		a.pop(x86::ebp);
		a.ret();
	}

	ASM_FUNCTION(R_AddCmdDrawText)
	{
		a.push(x86::ebp);
		a.mov(x86::ebp, x86::esp);
		a.pushad();

		a.mov(x86::ecx, x86::dword_ptr(x86::ebp, 0x2C));	// color
		a.push(x86::dword_ptr(x86::ebp, 0x28));				// style
		a.push(x86::dword_ptr(x86::ebp, 0x24));				// rotation
		a.push(x86::dword_ptr(x86::ebp, 0x20));				// yScale
		a.push(x86::dword_ptr(x86::ebp, 0x1C));				// xScale
		a.push(x86::dword_ptr(x86::ebp, 0x18));				// y
		a.push(x86::dword_ptr(x86::ebp, 0x14));				// x
		a.push(x86::dword_ptr(x86::ebp, 0x10));				// font
		a.push(x86::dword_ptr(x86::ebp, 0x0C));				// maxChars
		a.push(x86::dword_ptr(x86::ebp, 0x08));				// text
		a.call(0x5F6B00);
		a.add(x86::esp, 0x24);

		a.popad();
		a.pop(x86::ebp);
		a.ret();
	}

	ASM_FUNCTION(R_AddCmdDrawStretchPic)
	{
		a.push(x86::ebp);
		a.mov(x86::ebp, x86::esp);
		a.pushad();

		a.push(x86::dword_ptr(x86::ebp, 0x2C));				// color
		a.push(x86::dword_ptr(x86::ebp, 0x28));				// null4
		a.push(x86::dword_ptr(x86::ebp, 0x24));				// null3
		a.push(x86::dword_ptr(x86::ebp, 0x20));				// null2
		a.push(x86::dword_ptr(x86::ebp, 0x1C));				// null1
		a.push(x86::dword_ptr(x86::ebp, 0x18));				// h
		a.push(x86::dword_ptr(x86::ebp, 0x14));				// w
		a.push(x86::dword_ptr(x86::ebp, 0x10));				// y
		a.push(x86::dword_ptr(x86::ebp, 0x0C));				// x
		a.mov(x86::eax, x86::dword_ptr(x86::ebp, 0x08));	// material
		a.call(0x5F65F0);
		a.add(x86::esp, 0x24);

		a.popad();
		a.pop(x86::ebp);
		a.ret();
	}

	ASM_FUNCTION(R_TextWidth)
	{
		a.push(x86::ebp);
		a.mov(x86::ebp, x86::esp);
		a.pushad();

		a.push(x86::dword_ptr(x86::ebp, 0x10));				// font
		a.push(x86::dword_ptr(x86::ebp, 0x0C));				// maxChars
		a.mov(x86::eax, x86::dword_ptr(x86::ebp, 0x08));	// text
		a.call(0x5F1EE0);
		a.mov(x86::dword_ptr(x86::esp, 0x24), x86::eax);
		a.add(x86::esp, 0x08);

		a.popad();
		a.pop(x86::ebp);
		a.ret();
	}
}
