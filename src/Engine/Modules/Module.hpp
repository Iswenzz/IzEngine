#pragma once
#include "Engine/Backends/ImGUI.hpp"
#include "Engine/Backends/ImGUI/Components.hpp"
#include "Engine/Backends/ImGUI/Drawing/Window.hpp"
#include "Sys/Win32.hpp"

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
