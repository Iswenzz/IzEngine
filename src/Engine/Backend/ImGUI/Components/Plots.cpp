#include "Plots.hpp"

namespace IzEngine
{
	Plots::Plots()
	{
		SetRect(0, -100, 200, 100);
		SetRectAlignment(Horizontal::Left, Vertical::Bottom);
		SetDesigner(true);
	}
}
