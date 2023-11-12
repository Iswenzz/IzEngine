#pragma once
#include "Definitions.hpp"

#include "Game/Render/Render.hpp"
#include "Game/Render/GUI.hpp"
#include "Game/Player/Player.hpp"
#include "Modules/Modules.hpp"

#include "Sys/Log.hpp"
#include "Sys/Environment.hpp"
#include "Sys/Render/D3D9EX.hpp"

#include "Math/Math.hpp"
#include "Utils/Utils.hpp"
#include "Utils/Macros.hpp"

#include <memory>
#include <array>

namespace IW3SR
{
	/// <summary>
	/// IW3SR Game client.
	/// </summary>
	class Game
	{
	public:
		std::unique_ptr<class Log> Log;
		std::unique_ptr<class Render> Render;
		std::unique_ptr<class Modules> Modules;
		std::unique_ptr<class GUI> GUI;

		std::array<std::shared_ptr<class Player>, 64> Players;

		/// <summary>
		/// Initialize the game.
		/// </summary>
		Game();
		~Game();

		/// <summary>
		/// Initialize game.
		/// </summary>
		void Initialize();

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

extern Game* SR;
