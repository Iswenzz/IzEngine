#pragma once
#include "PMove.hpp"
#include "Game/Definitions.hpp"
#include "Engine/Core/Math.hpp"

namespace IW3SR::Game
{
	/// <summary>
	/// Player class.
	/// </summary>
	class API Player
	{
	public:
		centity_s* c;
		clientInfo_t* info;

		/// <summary>
		/// Initialize the player.
		/// </summary>
		Player() = default;

		/// <summary>
		/// Initialize the player.
		/// </summary>
		/// <param name="index">The player index.</param>
		Player(int index);
		~Player() = default;

		/// <summary>
		/// Is player self.
		/// </summary>
		/// <returns></returns>
		bool IsSelf();

		/// <summary>
		/// Is player alive.
		/// </summary>
		/// <returns></returns>
		bool IsAlive();

		/// <summary>
		/// Allocates all players.
		/// </summary>
		static void Allocates();

		/// <summary>
		/// Get all players.
		/// </summary>
		/// <returns></returns>
		static std::array<std::shared_ptr<Player>, 64>& GetAll();

		/// <summary>
		/// Get a player.
		/// </summary>
		/// <param name="index">The player index.</param>
		/// <returns></returns>
		static inline std::shared_ptr<Player> Get(int index);

		/// <summary>
		/// Check if the player is active.
		/// </summary>
		inline operator bool() const;

	private:
		static inline std::array<std::shared_ptr<Player>, 64> Players;
	};
}
