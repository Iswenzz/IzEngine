#include <string>
#include "Velocity.hpp"
#include "Game/Game.hpp"

namespace IW3SR
{
	Velocity::Velocity()
	{
		ID = "sr.player.velocity";
		Name = "Velocity";

		Pos = { -20, 20 };
		Size = { 0.6, 0.6 };
		Color = { 0, 1, 1, 1 };
	}

	void Velocity::OnDraw2D()
	{
		const int velocity = vec2(pmove->ps->velocity).Length();

		Draw2D::Text(std::to_string(velocity), g_fonts->objectiveFont, Pos, Size, 
			HORIZONTAL_ALIGN_CENTER, VERTICAL_ALIGN_TOP, Color);
	}
}
