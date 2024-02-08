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
		CLASS_SINGLETON(GameClient)
	public:
		/// <summary>
		/// Start the game.
		/// </summary>
		void Start();

		/// <summary>
		/// Shutdown the game.
		/// </summary>
		void Shutdown();

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

	private:
		/// <summary>
		/// Initialize the game.
		/// </summary>
		GameClient() = default;
		~GameClient() = default;
	};
}
