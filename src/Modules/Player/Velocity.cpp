#include <string>
#include "Velocity.hpp"
#include "Game/Game.hpp"

namespace IW3SR
{
	Velocity::Velocity()
	{
		ID = "sr.player.draw_velocity";
		Name = "Velocity";

		Pos = { 960, 540 };
		Size = { 2, 2 };
	}

	void Velocity::OnDraw2D()
	{
		const int velocity = vec2(pmove->ps->velocity).Length();
		const std::string velocityString = std::to_string(velocity);

		Draw2D::Text(velocityString, g_fonts->objectiveFont, Pos, Size);
	}
}