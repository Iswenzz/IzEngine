#include "VirtualScreen.hpp"

namespace IzEngine
{
	VirtualScreen::VirtualScreen(const vec2& size)
	{
		Size = size;
		Setup();
	}

	void VirtualScreen::Setup()
	{
		if (Size.x <= 0.0f || Size.y <= 0.0f)
		{
			AdjustedRealWidth = 0.0f;
			VirtualToReal = { 1, 1 };
			VirtualToFull = { 1, 1 };
			RealToVirtual = { 1, 1 };
			SubScreen = { 0, 0 };

			SafeArea();
			return;
		}
		AdjustedRealWidth = (4.0f / 3.0f) * Size.y;
		if (AdjustedRealWidth > Size.x)
			AdjustedRealWidth = Size.x;

		VirtualToReal = { AdjustedRealWidth / SCREEN_WIDTH, Size.y / SCREEN_HEIGHT };
		VirtualToFull = { Size.x / SCREEN_WIDTH, Size.y / SCREEN_HEIGHT };
		RealToVirtual = { SCREEN_WIDTH / AdjustedRealWidth, SCREEN_HEIGHT / Size.y };
		SubScreen = vec2{ Size.x - AdjustedRealWidth, 0 } * 0.5f;

		SafeArea();
	}

	void VirtualScreen::SafeArea()
	{
		RealMin = Position;
		RealMax = Position + Size;

		if (Size.x <= 0.0f || Size.y <= 0.0f || AdjustedRealWidth <= 0.0f)
		{
			VirtualMin = { 0, 0 };
			VirtualMax = { SCREEN_WIDTH, SCREEN_HEIGHT };
			return;
		}
		const float horizontalScale = SCREEN_WIDTH / AdjustedRealWidth;
		const float verticalScale = SCREEN_HEIGHT / Size.y;

		VirtualMin = { RealMin.x * horizontalScale, RealMin.y * verticalScale };
		VirtualMax = { RealMax.x * horizontalScale, RealMax.y * verticalScale };
	}

	void VirtualScreen::Apply(vec2& position, Horizontal horizontal, Vertical vertical)
	{
		vec2 zero = { 0, 0 };
		Apply(position, zero, horizontal, vertical);
	}

	void VirtualScreen::Apply(vec2& position, vec2& size, Horizontal horizontal, Vertical vertical)
	{
		float& x = position.x;
		float& y = position.y;
		float& w = size.x;
		float& h = size.y;

		switch (horizontal)
		{
		case Horizontal::SubLeft:
			x = VirtualToReal.x * x + SubScreen.x;
			w *= VirtualToReal.x;
			break;
		case Horizontal::Left:
			x = VirtualToReal.x * x + RealMin.x;
			w *= VirtualToReal.x;
			break;
		case Horizontal::Center:
			x = VirtualToReal.x * x + Size.x * 0.5f;
			w *= VirtualToReal.x;
			break;
		case Horizontal::Right:
			x = VirtualToReal.x * x + RealMax.x;
			w *= VirtualToReal.x;
			break;
		case Horizontal::Fullscreen:
			x = VirtualToFull.x * x;
			w *= VirtualToFull.x;
			break;
		case Horizontal::CenterSafeArea:
			x = (RealMax.x + RealMin.x) * 0.5f + VirtualToReal.x * x;
			w *= VirtualToReal.x;
			break;
		}
		switch (vertical)
		{
		case Vertical::Top:
			y = VirtualToReal.y * y + RealMin.y;
			h *= VirtualToReal.y;
			break;
		case Vertical::Center:
			y = VirtualToReal.y * y + Size.y * 0.5f;
			h *= VirtualToReal.y;
			break;
		case Vertical::Bottom:
			y = VirtualToReal.y * y + RealMax.y;
			h *= VirtualToReal.y;
			break;
		case Vertical::Fullscreen:
			y = VirtualToFull.y * y;
			h *= VirtualToFull.y;
			break;
		case Vertical::CenterSafeArea:
			y = VirtualToReal.y * y + (RealMax.y + RealMin.y) * 0.5f;
			h *= VirtualToReal.y;
			break;
		case Vertical::SubTop:
			y = VirtualToReal.y * y;
			h *= VirtualToReal.y;
			break;
		}
	}

	void VirtualScreen::Reverse(vec2& position, Horizontal horizontal, Vertical vertical)
	{
		vec2 zero = { 0, 0 };
		Reverse(position, zero, horizontal, vertical);
	}

	void VirtualScreen::Reverse(vec2& position, vec2& size, Horizontal horizontal, Vertical vertical)
	{
		float& x = position.x;
		float& y = position.y;
		float& w = size.x;
		float& h = size.y;

		switch (horizontal)
		{
		case Horizontal::SubLeft:
			x = (x - SubScreen.x) / VirtualToReal.x;
			w /= VirtualToReal.x;
			break;
		case Horizontal::Left:
			x = (x - RealMin.x) / VirtualToReal.x;
			w /= VirtualToReal.x;
			break;
		case Horizontal::Center:
			x = (x - Size.x * 0.5f) / VirtualToReal.x;
			w /= VirtualToReal.x;
			break;
		case Horizontal::Right:
			x = (x - RealMax.x) / VirtualToReal.x;
			w /= VirtualToReal.x;
			break;
		case Horizontal::Fullscreen:
			x /= VirtualToFull.x;
			w /= VirtualToFull.x;
			break;
		case Horizontal::CenterSafeArea:
			x = (x - (RealMax.x + RealMin.x) * 0.5f) / VirtualToReal.x;
			w /= VirtualToReal.x;
			break;
		}
		switch (vertical)
		{
		case Vertical::Top:
			y = (y - RealMin.y) / VirtualToReal.y;
			h /= VirtualToReal.y;
			break;
		case Vertical::Center:
			y = (y - Size.y * 0.5f) / VirtualToReal.y;
			h /= VirtualToReal.y;
			break;
		case Vertical::Bottom:
			y = (y - RealMax.y) / VirtualToReal.y;
			h /= VirtualToReal.y;
			break;
		case Vertical::Fullscreen:
			y /= VirtualToFull.y;
			h /= VirtualToFull.y;
			break;
		case Vertical::CenterSafeArea:
			y = (y - (RealMax.y + RealMin.y) * 0.5f) / VirtualToReal.y;
			h /= VirtualToReal.y;
			break;
		case Vertical::SubTop:
			y /= VirtualToReal.y;
			h /= VirtualToReal.y;
			break;
		}
	}
}
