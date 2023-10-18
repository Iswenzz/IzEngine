#include "FPS.hpp"
#include "Game/Dvar.hpp"

namespace IW3SR
{
	FPS::FPS()
	{
		ID = "sr.player.fps";
		Name = "FPS";

		Color = { 1, 1, 1, 1 };
		Pos = { 0, 20 };

		FPSText = Text("0", "0", Pos, 1, Color);
		FPSText.SetRectAlignment(HORIZONTAL_ALIGN_CENTER, VERTICAL_ALIGN_TOP);
		FPSText.SetAlignment(HUDALIGN_CENTER, HUDALIGN_BOTTOM);
	}

	void FPS::OnMenu()
	{	
		ImGui::ColorEdit4("Color", Color, ImGuiColorEditFlags_Float);

		ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.25f);
		ImGui::Combo("FPS font", &currentFont, fonts.data(), 8);
	}

	void FPS::OnDraw2D()
	{
		int FPS = Dvar::Get<int>("com_maxfps");
		FPSText.Value = std::to_string(FPS);
		FPSText.FontName = fonts[currentFont];
		FPSText.Color = Color;
		FPSText.Render();
	}
}