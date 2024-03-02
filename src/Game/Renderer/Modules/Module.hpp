#pragma once
#include "Game/Definitions.hpp"

#include "Engine/Backends/ImGUI.hpp"
#include "Engine/Backends/ImGUI/Drawing/Window.hpp"

namespace IW3SR::Game
{
	/// <summary>
	/// Module class.
	/// </summary>
	class API Module : public IInitializable
	{
	public:
		std::string ID;
		std::string Name;
		std::string Group;
		Window Menu;
		bool IsEnabled = false;

		/// <summary>
		/// Initialize the module.
		/// </summary>
		Module() = default;

		/// <summary>
		/// Initialize the module.
		/// </summary>
		/// <param name="id">The module ID.</param>
		/// <param name="name">The module name.</param>
		Module(const std::string& id, const std::string& name, const std::string& group);

		/// <summary>
		/// Release the module.
		/// </summary>
		virtual ~Module();

		/// <summary>
		/// Initialize the module.
		/// </summary>
		virtual void Initialize();

		/// <summary>
		/// Release the module.
		/// </summary>
		virtual void Release();

		/// <summary>
		/// Menu drawing.
		/// </summary>
		virtual void OnMenu();

		/// <summary>
		/// Draw 3D.
		/// </summary>
		virtual void OnDraw3D();

		/// <summary>
		/// Draw 3D.
		/// </summary>
		/// <param name="cmd">Render command.</param>
		/// <param name="viewInfo">View information for the graphics.</param>
		/// <param name="src">Source state for the graphics command buffer.</param>
		/// <param name="buf">Graphics command buffer state.</param>
		virtual void OnDraw3D(GfxCmdBufInput* cmd, GfxViewInfo* viewInfo, GfxCmdBufSourceState* src, GfxCmdBufState* buf);

		/// <summary>
		/// Draw 2D.
		/// </summary>
		virtual void OnDraw2D();

		/// <summary>
		/// Render frame.
		/// </summary>
		virtual void OnRender();

		/// <summary>
		/// Finish moving.
		/// </summary>
		/// <param name="cmd">The user command.</param>
		virtual void OnFinishMove(usercmd_s* cmd);

		/// <summary>
		/// Walk moving.
		/// </summary>
		/// <param name="pm">The player movement.</param>
		/// <param name="pml">The player movement library.</param>
		virtual void OnWalkMove(pmove_t* pm, pml_t* pml);

		/// <summary>
		/// Air moving.
		/// </summary>
		/// <param name="pm">The player movement.</param>
		/// <param name="pml">The player movement library.</param>
		virtual void OnAirMove(pmove_t* pm, pml_t* pml);

		/// <summary>
		/// On load position.
		/// </summary>
		virtual void OnLoadPosition();

		NLOHMANN_SERIALIZE_POLY_BASE(Module, IsEnabled, Menu)
	};
}
