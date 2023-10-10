#include "Draw2D.hpp"

namespace IW3SR
{
    void Draw2D::HUD(float x, float y, float w, float h, const vec4& color)
    {
        FillRect(x, y, w, h, color);
    }
}
