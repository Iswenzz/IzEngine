#pragma once
#include "Definitions.hpp"

#include "Game/Engine/Renderer.hpp"
#include "Game/Player/Player.hpp"
#include "Game/Sys/Console.hpp"

namespace IW3SR
{
	/// <summary>
	/// Game client.
	/// </summary>
	class GameClient
	{
	public:
		std::unique_ptr<Renderer> Renderer;
		std::unique_ptr<Console> Console;
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

#define GetRenderer()		GC->Renderer
#define GetGUI()			GC->Renderer->GUI
