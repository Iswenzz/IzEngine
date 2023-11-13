#include "Draw2D.hpp"

namespace IW3SR
{
    void Draw2D::Text(const std::string& text, const std::string& font, 
        float x, float y, float size, const vec4& color)
    {
        float w = size, h = size;
        ID3DXFont* f = Assets::Fonts[font];
        Math::ApplyRect(x, y, w, h, HORIZONTAL_ALIGN_LEFT, VERTICAL_ALIGN_TOP);
        RECT rect = { static_cast<int>(x), static_cast<int>(y), 0, 0 };
        f->DrawTextA(NULL, text.c_str(), -1, &rect, DT_NOCLIP, color);
    }
}
