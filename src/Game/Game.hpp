#pragma once
#include "Definitions.hpp"

#include "Game/Renderer/Renderer.hpp"
#include "Game/Player/Player.hpp"

#include "Game/Sys/Console.hpp"
#include "Game/Sys/Patch.hpp"
#include "Game/Sys/Sys.hpp"

namespace IW3SR::Game
{
	/// <summary>
	/// Application game client.
	/// </summary>
	class Application
	{
		CLASS_SINGLETON(Application)
	public:
		/// <summary>
		/// Start the application.
		/// </summary>
		void Start();

		/// <summary>
		/// Shutdown the application.
		/// </summary>
		void Shutdown();

		/// <summary>
		/// CoD4X client modifications.
		/// </summary>
		void CoD4X();

		/// <summary>
		/// Hook application.
		/// </summary>
		void Hook();

		/// <summary>
		/// Unhook application.
		/// </summary>
		void Unhook();

		/// <summary>
		/// Dispatch event.
		/// </summary>
		/// <param name="event">The event.s</param>
		void Dispatch(Event& event);

	private:
		/// <summary>
		/// Initialize the application.
		/// </summary>
		Application() = default;
		~Application() = default;
	};
}
