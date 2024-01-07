#pragma once
#include "Definitions.hpp"

#include "Engine/Modules/Modules.hpp"
#include "Engine/Modules/Features.hpp"

#include "Game/Engine/GUI.hpp"
#include "Game/Engine/Assets.hpp"
#include "Game/Engine/Render.hpp"
#include "Game/Player/Player.hpp"
#include "Game/Player/PMove.hpp"
#include "Game/Sys/Console.hpp"

#include "Math/Common.hpp"
#include "Utils/Utils.hpp"
#include "Sys/Win32.hpp"

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
		std::unique_ptr<class Console> Console;

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
