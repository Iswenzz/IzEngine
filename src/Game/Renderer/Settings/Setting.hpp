#pragma once
#include "Game/Definitions.hpp"

#include "Engine/Backends/ImGUI.hpp"
#include "Engine/Backends/ImGUI/Drawing/Window.hpp"

namespace IW3SR::Game
{
	/// <summary>
	/// Setting class.
	/// </summary>
	class API Setting : public IInitializable
	{
	public:
		std::string ID;
		std::string Name;
		std::string Group;
		Window Menu;

		/// <summary>
		/// Initialize the setting.
		/// </summary>
		Setting() = default;

		/// <summary>
		/// Initialize the setting.
		/// </summary>
		/// <param name="id">The setting ID.</param>
		/// <param name="name">The setting name.</param>
		Setting(const std::string& id, const std::string& name, const std::string& group);

		/// <summary>
		/// Release the setting.
		/// </summary>
		virtual ~Setting();

		/// <summary>
		/// Initialize the setting.
		/// </summary>
		virtual void Initialize();

		/// <summary>
		/// Release the setting.
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

		NLOHMANN_SERIALIZE_POLY_BASE(Setting, Menu)
	};
}
