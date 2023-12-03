#include "About.hpp"

namespace IW3SR::UI
{
	About::About() : Window("About") { }
	
	void About::Frame() 
	{
		if (!Open) return;

		Begin();
		End();
	}
}
