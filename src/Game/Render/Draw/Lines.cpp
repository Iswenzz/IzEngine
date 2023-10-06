#include "Lines.hpp"
#include "Sys/Memory.hpp"

namespace IW3SR
{
	Lines::Lines(int width, int limit, bool depthTest)
	{
		Width = width;
		Limit = limit;
		DepthTest = depthTest;

		Verts.resize(limit);
	}

	void test(float *a) {}

	void Lines::Add(const vec3& start, const vec3& end, const vec4& color)
	{
		if (Count + 2 > Limit)
			Count = 0;

		R_ConvertColorToBytes(color, Verts[Count].color);
		Memory::Reinterpret<int>(Verts[Count + 1].color, Verts[Count].color);
		Memory::Reinterpret<float*>(Verts[Count].xyz, start);
		Memory::Reinterpret<float*>(Verts[Count + 1].xyz, start);

		Count += 2;
	}

	void Lines::Frame()
	{
		RB_DrawLines3D(Count / 2, Width, Verts.data(), DepthTest);
		Count = 0;
	}
}
