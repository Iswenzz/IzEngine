#pragma once
#include "Definitions.hpp"

#include "Game/Render/Render.hpp"
#include "Game/Render/GUI.hpp"
#include "Game/Player/Player.hpp"
#include "Game/Player/PMove.hpp"
#include "Game/Sys/Features.hpp"
#include "Game/Sys/Modules.hpp"

#include "Sys/Log.hpp"
#include "Sys/Environment.hpp"
#include "Sys/Render/D3D9EX.hpp"
#include "Sys/Modules/DLLS.hpp"

#include "Math/Math.hpp"
#include "Utils/Utils.hpp"
#include "Utils/Macros.hpp"

namespace IW3SR
{
	/// <summary>
	/// IW3SR Game client.
	/// </summary>
	class Game
	{
	public:
		std::unique_ptr<class DLLS> DLLS;
		std::unique_ptr<class Features> Features;
		std::unique_ptr<class Modules> Modules;
		std::unique_ptr<class GUI> GUI;
		std::unique_ptr<class Render> Render;
		std::unique_ptr<class Log> Log;

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
