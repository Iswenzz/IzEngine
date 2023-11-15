#include "Draw2D.hpp"

namespace IW3SR
{
    void Draw2D::Text(const std::string& text, const std::string& font, float x, float y, const vec4& color)
    {
        ID3DXFont* f = Assets::Fonts.front();
        Math::ApplyRect(x, y, HORIZONTAL_ALIGN_LEFT, VERTICAL_ALIGN_TOP);
        RECT rect = { static_cast<int>(x), static_cast<int>(y), 0, 0 };
        f->DrawTextA(NULL, text.c_str(), -1, &rect, DT_NOCLIP, color);
    }
}
