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

	void Lines::Add(glm::vec3 start, glm::vec3 end, glm::vec4 color)
	{
		float colorArray[4];

		if (Count + 2 > Limit)
			Count = 0;
		glm::set_float4(colorArray, color);
		R_ConvertColorToBytes(colorArray, Verts[Count].color);
		Memory::Reinterpret<int>(Verts[Count + 1].color, Verts[Count].color);

		glm::set_float3(Verts[Count].xyz, start);
		glm::set_float3(Verts[Count + 1].xyz, end);

		Count += 2;
	}

	void Lines::Frame()
	{
		RB_DrawLines3D(Count / 2, Width, Verts.data(), DepthTest);
		Count = 0;
	}
}
