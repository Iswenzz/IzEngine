#include "Draw2D.hpp"

#include "ImGUI/UI.hpp"
#include "DX9/Assets.hpp"

namespace IW3SR::Engine
{
    void Draw2D::Text(const std::string& text, const std::string& font, float x, float y, const vec4& color)
    {
        const auto& f = Assets::Get().Fonts.begin()->second;
        UI::Get().Screen.Apply(x, y, HORIZONTAL_LEFT, VERTICAL_TOP);
        RECT rect = { static_cast<int>(x), static_cast<int>(y), 0, 0 };
        f->Base->DrawTextA(NULL, text.c_str(), -1, &rect, DT_NOCLIP, color.BGRA());
    }
}
