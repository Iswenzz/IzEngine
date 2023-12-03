#include "Binds.hpp"

namespace IW3SR::UI
{
	Binds::Binds() : Window("Binds") { }
	
	void Binds::Frame() 
	{
		if (!Open) return;

		Begin();
		End();
	}
}
