#pragma once
#include "Game/Definitions.hpp"

#include "Engine/Backends/ImGUI.hpp"
#include "Engine/Backends/ImGUI/Drawing/Window.hpp"

namespace IW3SR::Game
{
	/// <summary>
	/// Module class.
	/// </summary>
	class API Module : public IObject
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
		/// <param name="id">The ID.</param>
		/// <param name="group">The group.</param>
		/// <param name="name">The name.</param>
		Module(const std::string& id, const std::string& group, const std::string& name);

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
		/// Event dispatch.
		/// </summary>
		/// <param name="event">The event.</param>
		virtual void OnEvent(Event& event) override;

		/// <summary>
		/// Menu drawing.
		/// </summary>
		virtual void OnMenu();

		/// <summary>
		/// Walk moving.
		/// </summary>
		/// <param name="event">The event.</param>
		virtual void OnWalkMove(EventPMoveWalk& event);

		/// <summary>
		/// Air moving.
		/// </summary>
		/// <param name="event">The event.</param>
		virtual void OnAirMove(EventPMoveAir& event);

		/// <summary>
		/// Finish moving.
		/// </summary>
		/// <param name="event">The event.</param>
		virtual void OnFinishMove(EventPMoveFinish& event);

		/// <summary>
		/// On load position.
		/// </summary>
		virtual void OnLoadPosition();

		/// <summary>
		/// Draw 3D.
		/// </summary>
		/// <param name="event">The event.</param>
		virtual void OnDraw3D(EventRenderer3D& event);

		/// <summary>
		/// Draw 2D.
		/// </summary>
		/// <param name="event">The event.</param>
		virtual void OnDraw2D(EventRenderer2D& event);

		/// <summary>
		/// Render frame.
		/// </summary>
		virtual void OnRender();

		NLOHMANN_SERIALIZE_POLY_BASE(Module, IsEnabled, Menu)
	};
}
