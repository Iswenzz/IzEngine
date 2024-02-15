#include "VirtualScreen.hpp"

namespace IW3SR::Engine
{
    VirtualScreen::VirtualScreen(const vec2& position, const vec2& size, const vec2& display)
    {
        Position = position;
        Size = size;
        DisplaySize = display;

        Setup();
    }

    void VirtualScreen::Setup()
    {
        AdjustedRealWidth = (4.0f / 3.0f) * Size.y;
        if (AdjustedRealWidth > Size.x)
            AdjustedRealWidth = Size.x;

        VirtualToReal = { AdjustedRealWidth / 640.0f, Size.y / 480.0f };
        VirtualToFull = { Size.x / 640.0f, Size.y / 480.0f };
        RealToVirtual = { 640.0f / AdjustedRealWidth, 480.0f / Size.y };
        SubScreen = vec2{ Size.x - AdjustedRealWidth, 0 } * 0.5f;

        SafeArea();
    }

    void VirtualScreen::SafeArea()
    {
        const float horizontalAspectRatio = Size.x / AdjustedRealWidth;
        const float verticalAspectRatio = Size.y / Size.y;

        RealMin = Position;
        RealMax = Position + Size;

        VirtualMin = {
            RealMin.x * horizontalAspectRatio * (640.0f / Size.x),
            RealMin.y * verticalAspectRatio * (480.0f / Size.y)
        };
        VirtualMax = {
            RealMax.x * horizontalAspectRatio * (640.0f / Size.x),
            RealMax.y * verticalAspectRatio * (480.0f / Size.y)
        };
    }

    void VirtualScreen::Apply(float& x, float& y, Horizontal horizontal, Vertical vertical)
    {
        float zero = 0;
        Apply(x, y, zero, zero, horizontal, vertical);
    }

    void VirtualScreen::Apply(float& x, float& y, float& w, float& h, Horizontal horizontal, Vertical vertical)
    {
        switch (horizontal)
        {
        case HORIZONTAL_SUBLEFT:
            x = VirtualToReal.x * x + SubScreen.x;
            w *= VirtualToReal.x;
            break;
        case HORIZONTAL_LEFT:
            x = VirtualToReal.x * x + RealMin.x;
            w *= VirtualToReal.x;
            break;
        case HORIZONTAL_CENTER:
            x = VirtualToReal.x * x + Size.x * 0.5f;
            w *= VirtualToReal.x;
            break;
        case HORIZONTAL_RIGHT:
            x = VirtualToReal.x * x + RealMax.x;
            w *= VirtualToReal.x;
            break;
        case HORIZONTAL_FULLSCREEN:
            x = VirtualToFull.x * x;
            w *= VirtualToFull.x;
            break;
        case HORIZONTAL_CENTER_SAFEAREA:
            x = (RealMax.x + RealMin.x) * 0.5f + VirtualToReal.x * x;
            w *= VirtualToReal.x;
            break;
        }
        switch (vertical)
        {
        case VERTICAL_TOP:
            y = VirtualToReal.y * y + RealMin.y;
            h *= VirtualToReal.y;
            break;
        case VERTICAL_CENTER:
            y = VirtualToReal.y * y + Size.y * 0.5f;
            h *= VirtualToReal.y;
            break;
        case VERTICAL_BOTTOM:
            y = VirtualToReal.y * y + RealMax.y;
            h *= VirtualToReal.y;
            break;
        case VERTICAL_FULLSCREEN:
            y = VirtualToFull.y * y;
            h *= VirtualToFull.y;
            break;;
        case VERTICAL_CENTER_SAFEAREA:
            y = VirtualToReal.y * y + (RealMax.y + RealMin.y) * 0.5f;
            h *= VirtualToReal.y;
            break;
        case VERTICAL_SUBTOP:
            y = VirtualToReal.y * y;
            h *= VirtualToReal.y;
            break;
        }
    }

    void VirtualScreen::Reverse(float& x, float& y, Horizontal horizontal, Vertical vertical)
    {
        float zero = 0;
        Reverse(x, y, zero, zero, horizontal, vertical);
    }

    void VirtualScreen::Reverse(float& x, float& y, float& w, float& h, Horizontal horizontal, Vertical vertical)
    {
        switch (horizontal)
        {
        case HORIZONTAL_SUBLEFT:
            x = (x - SubScreen.x) / VirtualToReal.x;
            w /= VirtualToReal.x;
            break;
        case HORIZONTAL_LEFT:
            x = (x - RealMin.x) / VirtualToReal.x;
            w /= VirtualToReal.x;
            break;
        case HORIZONTAL_CENTER:
            x = (x - Size.x * 0.5f) / VirtualToReal.x;
            w /= VirtualToReal.x;
            break;
        case HORIZONTAL_RIGHT:
            x = (x - RealMax.x) / VirtualToReal.x;
            w /= VirtualToReal.x;
            break;
        case HORIZONTAL_FULLSCREEN:
            x /= VirtualToFull.x;
            w /= VirtualToFull.x;
            break;
        case HORIZONTAL_CENTER_SAFEAREA:
            x = (x - (RealMax.x + RealMin.x) * 0.5f) / VirtualToReal.x;
            w /= VirtualToReal.x;
            break;
        }
        switch (vertical)
        {
        case VERTICAL_TOP:
            y = (y - RealMin.y) / VirtualToReal.y;
            h /= VirtualToReal.y;
            break;
        case VERTICAL_CENTER:
            y = (y - Size.y * 0.5f) / VirtualToReal.y;
            h /= VirtualToReal.y;
            break;
        case VERTICAL_BOTTOM:
            y = (y - RealMax.y) / VirtualToReal.y;
            h /= VirtualToReal.y;
            break;
        case VERTICAL_FULLSCREEN:
            y /= VirtualToFull.y;
            h /= VirtualToFull.y;
            break;
        case VERTICAL_CENTER_SAFEAREA:
            y = (y - (RealMax.y + RealMin.y) * 0.5f) / VirtualToReal.y;
            h /= VirtualToReal.y;
            break;
        case VERTICAL_SUBTOP:
            y /= VirtualToReal.y;
            h /= VirtualToReal.y;
            break;
        }
    }
}
