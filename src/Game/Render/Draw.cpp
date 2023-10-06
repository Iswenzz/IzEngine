#include "Draw.hpp"
#include "Sys/Memory.hpp"

namespace IW3SR
{
    void Draw::Line(const vec3& start, const vec3& end, const vec4& color)
    {
		if (LinesCount + 2 > 2735)
			LinesCount = 0;

		R_ConvertColorToBytes(color, LinesVerts[LinesCount].color);
		Memory::Reinterpret<int>(LinesVerts[LinesCount + 1].color, LinesVerts[LinesCount].color);

		float* vertA = LinesVerts[LinesCount].xyz;
		float* vertB = LinesVerts[LinesCount + 1].xyz;
		vertA = start;
		vertB = end;

		LinesCount += 2;
    }

	void Draw::Rect2D(float x, float y, float w, float h, const vec4& color)
	{
		FillRect(x, y, w, h, color);
	}

	void Draw::Frame()
	{
		RB_DrawLines3D(LinesCount / 2, LinesWidth, LinesVerts.data(), LinesDepthTest);
		LinesCount = 0;
	}
}
