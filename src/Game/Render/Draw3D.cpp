#include "Draw3D.hpp"
#include "Sys/Memory.hpp"

namespace IW3SR
{
    void Draw3D::Line(const vec3& start, const vec3& end, const vec4& color)
    {
        if (LinesCount + 2 > 2735)
            LinesCount = 0;

        R_ConvertColorToBytes(color, LinesVerts[LinesCount].color);
        Memory::Assign<int>(LinesVerts[LinesCount + 1].color, LinesVerts[LinesCount].color);
        Memory::Assign<3, float>(LinesVerts[LinesCount].xyz, start);
        Memory::Assign<3, float>(LinesVerts[LinesCount + 1].xyz, end);

        LinesCount += 2;
    }

    void Draw3D::Frame()
    {
        RB_DrawLines3D(LinesCount / 2, LinesWidth, LinesVerts.data(), LinesDepthTest);
        LinesCount = 0;
    }
}
