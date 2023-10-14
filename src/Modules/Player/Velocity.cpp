#include <string>
#include "Velocity.hpp"
#include "Game/Game.hpp"

namespace IW3SR
{
	Velocity::Velocity()
	{
		ID = "sr.player.velocity";
		Name = "Velocity";

		Color = { 0, 1, 1, 1 };

		VelocityText = Text("0", FONT_OBJECTIVE, 0, 2, 0.6, Color);
		VelocityText.SetRectAlignment(HORIZONTAL_ALIGN_CENTER, VERTICAL_ALIGN_TOP);
		VelocityText.SetAlignment(HUDALIGN_CENTER, HUDALIGN_BOTTOM);
	}

	void Velocity::OnDraw2D()
	{
		const int velocity = vec2(pmove->ps->velocity).Length();
		VelocityText.Value = std::to_string(velocity);
		VelocityText.Render();
	}
}
