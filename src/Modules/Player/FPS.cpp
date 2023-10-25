#include "FPS.hpp"
#include "Game/Dvar.hpp"
#include "Game/Render/Draw2D.hpp"

namespace IW3SR
{
	FPS::FPS() : Module("sr.player.fps", "FPS")
	{
		Color = { 1, 1, 1, 1 };
		Position = { 0, 20 };

		FPSText = Text("0", FONT_BIG_DEV, Position, 1, Color);
		FPSText.SetRectAlignment(HORIZONTAL_ALIGN_CENTER, VERTICAL_ALIGN_TOP);
		FPSText.SetAlignment(HUDALIGN_CENTER, HUDALIGN_BOTTOM);
	}

	void FPS::OnMenu()
	{	
		ImGui::ColorEdit4("Color", Color, ImGuiColorEditFlags_Float);
		ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.35f);

		if (ImGui::Combo("Font", &FPSText.FontIndex, Draw2D::Fonts.data(), Draw2D::Fonts.size()))
			FPSText.SetFont(Draw2D::Fonts[FPSText.FontIndex]);
	}

	void FPS::OnDraw2D()
	{
		int FPS = Dvar::Get<int>("com_maxfps");
		FPSText.Value = std::to_string(FPS);
		FPSText.Color = Color;
		FPSText.Render();
	}
}