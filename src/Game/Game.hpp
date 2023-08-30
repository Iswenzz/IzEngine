#pragma once
#include "Definitions.hpp"
#include "Modules/Modules.hpp"
#include "Modules/Sys/Console.hpp"
#include "Sys/Log.hpp"

#include <memory>

namespace IW3SR
{
	/// <summary>
	/// IW3SR Game client.
	/// </summary>
	class Game
	{
	public:
		std::unique_ptr<Log> Log;
		std::unique_ptr<Modules> Modules;

		/// <summary>
		/// Initialize the game.
		/// </summary>
		Game();
		~Game();

		/// <summary>
		/// Patch for clients.
		/// </summary>
		void Patch();

		/// <summary>
		/// Game frame.
		/// </summary>
		void Frame();
	};
}
