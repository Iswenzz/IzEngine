#pragma once
#include "Game/Definitions.hpp"
#include "Game/Sys/Dvar.hpp"

#include "Game/Player/Player.hpp"
#include "Game/Player/PMove.hpp"

#include "Game/Render/Assets.hpp"
#include "Game/Render/Drawing/Draw2D.hpp"
#include "Game/Render/Drawing/Draw3D.hpp"
#include "Game/Render/Drawing/Window.hpp"
#include "Game/Render/Drawing/HUD.hpp"
#include "Game/Render/Drawing/Text.hpp"
#include "Game/Render/ImGUI.hpp"

#include "Game/Engine/Assets.hpp"
#include "Game/Engine/Drawing/Draw2D.hpp"
#include "Game/Engine/Drawing/Draw3D.hpp"
#include "Game/Engine/Drawing/HUD.hpp"
#include "Game/Engine/Drawing/Text.hpp"
#include "Game/Engine/Drawing/Lines.hpp"

#include "Math/Math.hpp"
#include "Utils/Macros.hpp"

#include "Sys/Environment.hpp"
#include "Sys/Listeners/KeyListener.hpp"
#include "Sys/Log.hpp"

namespace IW3SR
{
	/// <summary>
	/// Game module.
	/// </summary>
	class API Module
	{
	public:
		std::string ID;
		std::string Name;
		std::string Group;
		Window Menu;
		bool IsEnabled;

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
		/// Shutdown the module.
		/// </summary>
		virtual void Shutdown();

		/// <summary>
		/// Menu drawing.
		/// </summary>
		virtual void OnMenu();

		/// <summary>
		/// Engine 3D.
		/// </summary>
		virtual void OnDraw3D();

		/// <summary>
		/// Engine 2D.
		/// </summary>
		virtual void OnDraw2D();

		/// <summary>
		/// Render frame.
		/// </summary>
		virtual void OnFrame();

		/// <summary>
		/// Finish moving.
		/// </summary>
		/// <param name="cmd">The user command.</param>
		virtual void OnFinishMove(usercmd_s* cmd);

		NLOHMANN_SERIALIZE_POLY_BASE(Module, IsEnabled, Menu)
	};
}
