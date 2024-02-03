#pragma once
#include "Definitions.hpp"

#include "Game/Engine/Renderer.hpp"
#include "Game/Player/Player.hpp"

#include "Game/Sys/Console.hpp"
#include "Game/Sys/Patch.hpp"
#include "Game/Sys/Sys.hpp"

namespace IW3SR::Game
{
	/// <summary>
	/// Game client.
	/// </summary>
	class GameClient
	{
	public:
		std::array<std::shared_ptr<Player>, 64> Players;

		/// <summary>
		/// Initialize the game.
		/// </summary>
		GameClient();
		~GameClient();

		/// <summary>
		/// Start game.
		/// </summary>
		void Start();

		/// <summary>
		/// CoD4X patch.
		/// </summary>
		void CoD4X();

		/// <summary>
		/// Hook game.
		/// </summary>
		void Hook();

		/// <summary>
		/// Unhook game.
		/// </summary>
		void Unhook();
	};
}

extern GameClient* GC;
