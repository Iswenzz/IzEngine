#include "Plots.hpp"

namespace IzEngine
{
	Plots::Plots()
	{
		SetRect(0, -100, 200, 100);
		SetRectAlignment(HORIZONTAL_LEFT, VERTICAL_BOTTOM);
		SetDesigner(true);
	}
}
