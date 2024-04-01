#include "Hooks.hpp"

#include "Game/Renderer/Backends/D3D9.hpp"
#include "Game/Renderer/Modules/Modules.hpp"
#include "Game/Renderer/Renderer.hpp"
#include "Game/Renderer/Settings/Settings.hpp"

#include "Game/Player/Player.hpp"
#include "Game/System/Console.hpp"
#include "Game/System/System.hpp"

// clang-format off
namespace IW3SR::Game
{
	Hook<HWND STDCALL(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName,
		DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu,
		HINSTANCE hInstance, LPVOID lpParam)>
		CreateWindowExA_h(CreateWindowExA, System::CreateMainWindow);

	Hook<IDirect3D9* STDCALL(UINT sdk)>
		Direct3DCreate9_h(Direct3DCreate9, D3D9::Direct3DCreate9);

	Hook<LRESULT CALLBACK(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)>
		MainWndProc_h(0x57BB20, System::MainWndProc);

	Hook<void(int localClientNum, int controllerIndex, char* command)>
		Cmd_ExecuteSingleCommand_h(0x4F9AB0, Console::ExecuteSingleCommand);

	Hook<void(ConChannel channel, const char* msg, int type)>
		Com_PrintMessage_h(0x4FCA50, Console::Write);

	Hook<void(int localClientNum)>
		CG_DrawCrosshair_h(0x4311A0, GRenderer::Draw2D);

	Hook<void(int localClientNum)>
		CG_PredictPlayerState_Internal_h(0x447260, Player::Predict);

	Hook<void(usercmd_s* cmd)>
		CL_FinishMove_h(0x463A60, PMove::FinishMove);

	Hook<void(pmove_t* pm, pml_t* pml)>
		PM_WalkMove_h(0x40F7A0, PMove::WalkMove);

	Hook<void(pmove_t* pm, pml_t* pml)>
		PM_AirMove_h(0x40F680, PMove::AirMove);

	Hook<void()>
		R_Init_h(0x5F4EE0, GRenderer::Initialize);

	Hook<void(void* cmds)>
		R_RenderAllLeftovers_h(0x6156EC, ASM_LOAD(R_RenderAllLeftovers_h));

	Hook<void(int window)>
		R_Shutdown_h(0x5F4F90, GRenderer::Shutdown);

	Hook<void(GfxCmdBufInput* cmd, GfxViewInfo* viewInfo, GfxCmdBufSourceState* src, GfxCmdBufState* buf)>
		RB_EndSceneRendering_h(0x6496EC, GRenderer::Draw3D);
}
// clang-format on
namespace IW3SR::Game
{
	using namespace asmjit;

	ASM_FUNCTION(R_RenderAllLeftovers_h)
	{
		a.push(x86::ebp);
		a.mov(x86::ebp, x86::esp);
		a.pushad();

		a.push(x86::eax); // cmds
		a.call(GRenderer::Commands);
		a.add(x86::esp, 4);

		a.popad();
		a.pop(x86::ebp);
		a.call(x86::dword_ptr(uintptr_t(&R_RenderAllLeftovers_h.Trampoline)));
		a.ret();
	}
}
