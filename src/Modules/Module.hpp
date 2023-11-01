#pragma once
#include "Utils/Macros.hpp"
#include "Utils/Math.hpp"
#include "Utils/ImGUI.hpp"

#include "Game/Render/Draw.hpp"
#include "Game/Render/Draw2D.hpp"
#include "Game/Render/Draw3D.hpp"
#include "Game/Render/Draw/HUD.hpp"
#include "Game/Render/Draw/Text.hpp"
#include "Game/Render/Draw/Lines.hpp"
#include "Game/Render/Draw/Window.hpp"

#include "Sys/Environment.hpp"

#include <string>
#include <memory>

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
		Window Menu;
		bool IsEnabled;

		/// <summary>
		/// Initialize the module.
		/// </summary>
		/// <param name="id">The module ID.</param>
		/// <param name="name">The module name.</param>
		Module(const std::string& id, const std::string& name);

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
		/// Draw 3D callback.
		/// </summary>
		virtual void OnDraw3D();

		/// <summary>
		/// Draw 2D callback.
		/// </summary>
		virtual void OnDraw2D();

		/// <summary>
		/// Render frame callback.
		/// </summary>
		virtual void OnFrame();

		NLOHMANN_DEFINE_POLY_BASE(Module, IsEnabled, Menu);
	};
}
