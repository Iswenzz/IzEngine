#include "FPS.hpp"

namespace IW3SR
{
	FPS::FPS() : Module("sr.player.fps", "FPS", "Player")
	{
		Color = { 1, 1, 1, 1 };
		Position = { 0, 20 };

		FPSText = Engine::Text("0", FONT_BIG_DEV, Position, 1, Color);
		FPSText.SetRectAlignment(HORIZONTAL_ALIGN_CENTER, VERTICAL_ALIGN_TOP);
		FPSText.SetAlignment(HUDALIGN_CENTER, HUDALIGN_BOTTOM);
	}

	void FPS::OnMenu()
	{	
		ImGui::ColorEdit4("Color", Color, ImGuiColorEditFlags_Float);
		ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.35f);

		const std::vector<const char*>& fonts = Engine::Assets::FontNames;
		if (ImGui::Combo("Font", &FPSText.FontIndex, fonts.data(), fonts.size()))
			FPSText.SetFont(fonts[FPSText.FontIndex]);
	}

	void FPS::OnDraw2D()
	{
		int FPS = Dvar::Get<int>("com_maxfps");
		FPSText.Value = std::to_string(FPS);
		FPSText.Color = Color;
		FPSText.Render();
	}
}