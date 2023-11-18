#pragma once
#include "Game/Definitions/Structs.hpp"

namespace IW3SR
{
	/// <summary>
	/// All features including the player movement.
	/// </summary>
	class PMove
	{
	public:
		/// <summary>
		/// usercmd_s creation hook function.
		/// </summary>
		/// <param name="cmd">The client command.</param>
		static void FinishMove(usercmd_s* cmd);
	};
}