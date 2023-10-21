#pragma once
#include "Utils/Math.hpp"
#include "Utils/Macros.hpp"

#include <string>
#include <memory>

namespace IW3SR
{
	/// <summary>
	/// Game module.
	/// </summary>
	class Module
	{
	public:
		std::string ID;
		std::string Name;
		bool IsEnabled;
		bool MenuOpen = false;
		vec2 MenuSize = vec2::Zero;
		vec2 MenuPosition = vec2::Zero;

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

		NLOHMANN_DEFINE_POLY_BASE(Module, IsEnabled, MenuSize, MenuPosition);
	};
}
