#include "About.hpp"

namespace IW3SR::UI
{
	About::About() : Window("About") { }
	
	void About::Frame() 
	{
		if (!Open) return;

		constexpr const char* IW3SR = "IW3SR";
		constexpr const char* markdown = R"(
A Call of Duty 4 Modification.

[https://github.com/Iswenzz/IW3SR](https://github.com/Iswenzz/IW3SR)
IW3SR (c) 2023-2024)";

		Begin();
		ImGui::PushFont(GUI::Themes.H1);
		ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize(IW3SR).x) * 0.5f);
		ImGui::Text(IW3SR);
		ImGui::Separator();
		ImGui::PopFont();

		ImGui::Markdown(markdown);
		End();
	}
}
