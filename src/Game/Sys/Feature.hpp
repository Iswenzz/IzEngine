#pragma once
#include "Game/Definitions.hpp"

#include "Game/Sys/Dvar.hpp"
#include "Game/Player/Player.hpp"
#include "Game/Render/Assets.hpp"
#include "Game/Render/Drawing/Window.hpp"
#include "Game/Render/ImGUI.hpp"
#include "Game/Engine/Assets.hpp"

#include "Math/Math.hpp"
#include "Utils/Macros.hpp"

#include "Sys/Environment.hpp"
#include "Sys/Listeners/KeyListener.hpp"

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
