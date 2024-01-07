#pragma once
#include "Engine/Backends/ImGUI.hpp"
#include "Engine/Backends/ImGUI/Components.hpp"
#include "Engine/Backends/ImGUI/Drawing/Window.hpp"
#include "Sys/Win32.hpp"

namespace IW3SR
{
	/// <summary>
	/// Game feature.
	/// </summary>
	class API Feature
	{
	public:
		std::string ID;
		std::string Name;
		std::string Group;
		Window Menu;

		/// <summary>
		/// Initialize the feature.
		/// </summary>
		Feature() = default;

		/// <summary>
		/// Initialize the feature.
		/// </summary>
		/// <param name="id">The feature ID.</param>
		/// <param name="name">The feature name.</param>
		Feature(const std::string& id, const std::string& name, const std::string& group);

		/// <summary>
		/// Release the feature.
		/// </summary>
		virtual ~Feature();

		/// <summary>
		/// Initialize the feature.
		/// </summary>
		virtual void Initialize();

		/// <summary>
		/// Shutdown the feature.
		/// </summary>
		virtual void Shutdown();

		/// <summary>
		/// Menu drawing.
		/// </summary>
		virtual void OnMenu();

		NLOHMANN_SERIALIZE_POLY_BASE(Feature, Menu)
	};
}
