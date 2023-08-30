#pragma once
#include "Definitions.hpp"

namespace IW3SR
{
	class Game
	{
	public:
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
