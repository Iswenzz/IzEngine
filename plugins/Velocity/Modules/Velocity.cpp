#include "Velocity.hpp"
#include "Game/Game.hpp"

namespace IW3SR
{
	Velocity::Velocity() : Module("sr.player.velocity", "Velocity")
	{
		Color = { 0, 1, 1, 1 };

		VelocityText = Engine::Text("0", FONT_OBJECTIVE, 0, 2, 0.6, Color);
		VelocityText.SetRectAlignment(HORIZONTAL_ALIGN_CENTER, VERTICAL_ALIGN_TOP);
		VelocityText.SetAlignment(HUDALIGN_CENTER, HUDALIGN_BOTTOM);
	}

	void Velocity::OnMenu()
	{
		ImGui::ColorEdit4("Color", Color, ImGuiColorEditFlags_Float);

		const std::vector<const char*>& fonts = Engine::Draw2D::FontNames;
		if (ImGui::Combo("Font", &VelocityText.FontIndex, fonts.data(), fonts.size()))
			VelocityText.SetFont(fonts[VelocityText.FontIndex]);
	}

	void Velocity::OnDraw2D()
	{
		const int velocity = vec2(pmove->ps->velocity).Length();
		VelocityText.Value = std::to_string(velocity);
		VelocityText.Color = Color;
		VelocityText.Render();
	}
}
