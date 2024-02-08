#include "Lines.hpp"

namespace IW3SR::Game
{
	Lines::Lines(int width, int max, bool depthTest)
	{
		Width = width;
		MaxVertex = max;
		DepthTest = depthTest;
		Verts.resize(max);
	}

	void Lines::Add(const vec3& start, const vec3& end, const vec4& color)
	{
		if (Vertex + 2 > MaxVertex)
			Vertex = 0;

		R_ConvertColorToBytes(color, Verts[Vertex].color);
		Memory::Assign<int>(Verts[Vertex + 1].color, Verts[Vertex].color);
		Memory::AssignArray<3, float>(Verts[Vertex].xyz, start);
		Memory::AssignArray<3, float>(Verts[Vertex + 1].xyz, end);

		Vertex += 2;
	}

	void Lines::AddBox(const vec3& position, const vec3& size, const vec4& color)
	{
		vec3 tfl = { position.x + size.x, position.y - size.y, position.z + size.z };
		vec3 tfr = { position.x + size.x, position.y + size.y, position.z + size.z };
		vec3 tbl = { position.x - size.x, position.y + size.y, position.z + size.z };
		vec3 tbr = { position.x - size.x, position.y - size.y, position.z + size.z };

		vec3 bfl = { position.x + size.x, position.y - size.y, position.z };
		vec3 bfr = { position.x + size.x, position.y + size.y, position.z };
		vec3 bbl = { position.x - size.x, position.y + size.y, position.z };
		vec3 bbr = { position.x - size.x, position.y - size.y, position.z };

		Add(tbl, tfr, color);
		Add(tfl, tfr, color);
		Add(tfl, tbr, color);
		Add(tbl, tbr, color);

		Add(bbl, bfr, color);
		Add(bfr, bfl, color);
		Add(bfl, bbr, color);
		Add(bbr, bbl, color);

		Add(bbr, tbr, color);
		Add(bbl, tbl, color);
		Add(bfr, tfr, color);
		Add(bfl, tfl, color);
	}

	void Lines::Render()
	{
		if (Vertex < 2)
			return;
		
		RB_DrawLines3D(Vertex / 2, Width, Verts.data(), DepthTest);
		Vertex = 0;
	}
}
