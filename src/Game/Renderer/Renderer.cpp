#include "Renderer.hpp"
#include "Assets.hpp"
#include "GUI.hpp"

#include "Backends/D3D9EX.hpp"
#include "Drawing/Draw2D.hpp"
#include "Drawing/Draw3D.hpp"
#include "Settings/Settings.hpp"
#include "Modules/Modules.hpp"

namespace IW3SR::Game
{
	void Renderer::Initialize()
	{
		R_Init_h();

		Device::Get().Assign(dx->d3d9, dx->device);
		Device::Get().CreateScreen();

		Assets::Get().Initialize();
		GUI::Get().Initialize();
		Modules::Get().Initialize();
		Settings::Get().Initialize();

		EventPluginRenderer event;
		Plugins::Dispatch(event);
	}

	void Renderer::Shutdown(int window)
	{
		Settings::Get().Release();
		Modules::Get().Release();
		GUI::Get().Release();
		Assets::Get().Release();

		R_Shutdown_h(window);
	}

	void Renderer::Draw2D(int localClientNum)
	{
		CG_DrawCrosshair_h(localClientNum);

		EventRenderer2D event;
		Application::Get().Dispatch(event);
	}

	void Renderer::Draw3D(GfxCmdBufInput* cmd, GfxViewInfo* viewInfo, GfxCmdBufSourceState* src, GfxCmdBufState* buf)
	{
		RB_EndSceneRendering_h(cmd, viewInfo, src, buf);
		Draw3D::Render();

		EventRenderer3D event;
		Application::Get().Dispatch(event);
	}

	void Renderer::Commands(void* cmds)
	{
		// HLSL offline gameTime constants
		if (client_ui->connectionState != CA_ACTIVE)
			R_SetGameTime(gfx_cmdBufSourceState, UI::Get().Time());
	}

	void Renderer::Render()
	{
		UI::Get().Begin();
		GUI::Get().Render();

		if (client_ui->connectionState == CA_ACTIVE)
		{
			EventRendererRender event;
			Application::Get().Dispatch(event);
		}

		UI::Get().End();
		KeyListener::Reset();
	}
}
