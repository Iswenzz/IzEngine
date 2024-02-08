#include "Draw3D.hpp"

namespace IW3SR::Game
{
	void Draw3D::Box(const vec3& position, const vec3& size, const vec4& color)
    {
        vec3 tfl = { position.x + size.x, position.y - size.y, position.z + size.z };
        vec3 tfr = { position.x + size.x, position.y + size.y, position.z + size.z };
        vec3 tbl = { position.x - size.x, position.y + size.y, position.z + size.z };
        vec3 tbr = { position.x - size.x, position.y - size.y, position.z + size.z };

        vec3 bfl = { position.x + size.x, position.y - size.y, position.z };
        vec3 bfr = { position.x + size.x, position.y + size.y, position.z };
        vec3 bbl = { position.x - size.x, position.y + size.y, position.z };
        vec3 bbr = { position.x - size.x, position.y - size.y, position.z };

        Line(tbl, tfr, color);
        Line(tfl, tfr, color);
        Line(tfl, tbr, color);
        Line(tbl, tbr, color);

        Line(bbl, bfr, color);
        Line(bfr, bfl, color);
        Line(bfl, bbr, color);
        Line(bbr, bbl, color);

        Line(bbr, tbr, color);
        Line(bbl, tbl, color);
        Line(bfr, tfr, color);
        Line(bfl, tfl, color);
    }

    void Draw3D::Line(const vec3& start, const vec3& end, const vec4& color)
    {
        if (LinesVertex + 2 > LinesMaxVertex)
            LinesVertex = 0;

        R_ConvertColorToBytes(color, LinesVerts[LinesVertex].color);
        Memory::Assign<int>(LinesVerts[LinesVertex + 1].color, LinesVerts[LinesVertex].color);
        Memory::AssignArray<3, float>(LinesVerts[LinesVertex].xyz, start);
        Memory::AssignArray<3, float>(LinesVerts[LinesVertex + 1].xyz, end);

        LinesVertex += 2;
    }

    void Draw3D::Render()
    {
        if (LinesVertex >= 2)
        {
            RB_DrawLines3D(LinesVertex / 2, LinesWidth, LinesVerts.data(), LinesDepthTest);
            LinesVertex = 0;
        }
    }
}
