#include "About.hpp"

namespace IW3SR::UI
{
	About::About() : Window("About") { }
	
	void About::Frame() 
	{
		if (!Open) return;

		Begin();
		ImGui::Text("IW3SR (c) 2023-2024");
		ImGui::Text("https://github.com/Iswenzz/IW3SR");
		End();
	}
}
