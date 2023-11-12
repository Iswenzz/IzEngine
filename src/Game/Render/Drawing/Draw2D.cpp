#include "Draw2D.hpp"

namespace IW3SR
{
	void Draw2D::Initialize()
	{
        HDC hdc = GetDC(NULL);
        int numFonts = GetFontData(hdc, 0, 0, NULL, 0);
        std::vector<LOGFONT> fonts(numFonts);
        GetFontData(hdc, 0, 0, fonts.data(), numFonts * sizeof(LOGFONT));

        for (const LOGFONT& font : fonts)
        {
            ID3DXFont* pFont;
            D3DXCreateFont(dx->device, 20, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, font.lfFaceName, &pFont);

            Fonts[font.lfFaceName] = pFont;
            FontNames.push_back(font.lfFaceName);
        }
        ReleaseDC(NULL, hdc);
	}

    void Draw2D::Text(const std::string& text, const std::string& font, 
        float x, float y, float size, const vec4& color)
    {
        
    }
}
