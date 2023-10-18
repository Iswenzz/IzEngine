#include "FPS.hpp"
#include "Game/Dvar.hpp"

namespace IW3SR
{
	FPS::FPS()
	{
		ID = "sr.player.fps";
		Name = "FPS";
		MenuSize = { 400, 100 };

		Color = { 1, 1, 1, 1 };
		Pos = { 0, 20 };

		FPSText = Text("0", FONT_BIG_DEV, Pos, 1, Color);
		FPSText.SetRectAlignment(HORIZONTAL_ALIGN_CENTER, VERTICAL_ALIGN_TOP);
		FPSText.SetAlignment(HUDALIGN_CENTER, HUDALIGN_BOTTOM);
	}

	void FPS::OnMenu()
	{	
		ImGui::ColorEdit4("Color", Color, ImGuiColorEditFlags_Float);
		
		ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.35f);

		if (ImGui::Combo("FPS font", &currentFont, fonts.data(), 8))
			FPSText.SetFont(fonts[currentFont]);
	}

	void FPS::OnDraw2D()
	{
		int FPS = Dvar::Get<int>("com_maxfps");
		FPSText.Value = std::to_string(FPS);
		FPSText.Color = Color;
		FPSText.Render();
	}
}