#include "Lines.hpp"

namespace IW3SR::Game
{
	Lines::Lines(int width, int limit, bool depthTest)
	{
		Width = width;
		Limit = limit;
		DepthTest = depthTest;
		Verts.resize(limit);
	}

	void Lines::Add(const vec3& start, const vec3& end, const vec4& color)
	{
		if (Count + 2 > Limit)
			Count = 0;

		R_ConvertColorToBytes(color, Verts[Count].color);
		Memory::Assign<int>(Verts[Count + 1].color, Verts[Count].color);
		Memory::AssignArray<3, float>(Verts[Count].xyz, start);
		Memory::AssignArray<3, float>(Verts[Count + 1].xyz, end);

		Count += 2;
	}

	void Lines::Render()
	{
		RB_DrawLines3D(Count / 2, Width, Verts.data(), DepthTest);
		Count = 0;
	}
}
