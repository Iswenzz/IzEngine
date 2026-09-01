#include "Notification.hpp"

#include "Engine/Backend/ImGUI/Common.hpp"

namespace IzEngine
{
	// Everything is sized off the current font instead of the virtual screen, so a toast keeps its
	// proportions to the text at any resolution the menu is scaled to.
	constexpr float MarginScale = 0.75f;
	constexpr float PaddingScale = 0.5f;
	constexpr float SpacingScale = 0.35f;
	constexpr float AccentScale = 0.16f;
	constexpr float RoundingScale = 0.25f;
	constexpr float MinWidthScale = 9.0f;

	// Slide and fade occupy the first and last SlideTime seconds of a toast's life. The stack eases
	// at its own rate, which is what keeps a removal from teleporting the entries below it.
	constexpr float SlideTime = 0.25f;
	constexpr float StackSpeed = 12.0f;

	constexpr ImU32 NotificationBackground = IM_COL32(6, 6, 8, 235);
	constexpr ImU32 NotificationBorder = IM_COL32(255, 255, 255, 26);
	constexpr ImU32 NotificationText = IM_COL32(236, 236, 240, 255);

	float EaseOut(float t)
	{
		t = std::clamp(t, 0.0f, 1.0f);
		const float inverse = 1.0f - t;
		return 1.0f - inverse * inverse * inverse;
	}

	ImU32 WithAlpha(ImU32 color, float alpha)
	{
		const auto channel = static_cast<ImU32>(((color >> IM_COL32_A_SHIFT) & 0xFF) * std::clamp(alpha, 0.0f, 1.0f));
		return (color & ~IM_COL32_A_MASK) | (channel << IM_COL32_A_SHIFT);
	}

	void Notifications::Push(const std::string& message, NotificationLevel level, float duration, float delay)
	{
		if (message.empty())
			return;

		// Dropping the oldest keeps a spamming caller from filling the screen; the survivors ease
		// upwards into the freed slot on the next frame.
		while (List.size() >= Max)
			List.erase(List.begin());

		List.push_back({ message, level, UI::Time() + std::max(0.0f, delay), duration });
	}

	void Notifications::Clear()
	{
		List.clear();
	}

	void Notifications::Render()
	{
		if (List.empty())
			return;

		const double time = UI::Time();
		const float delta = UI::DeltaTime();
		const float font = ImGui::GetFontSize();

		const float margin = font * MarginScale;
		const float padding = font * PaddingScale;
		const float spacing = font * SpacingScale;
		const float accent = std::max(2.0f, font * AccentScale);
		const float rounding = font * RoundingScale;

		// The foreground list draws over every window, so a toast is never hidden by the menu it is
		// telling the user about.
		ImDrawList* draw = ImGui::GetForegroundDrawList();
		float target = margin;

		for (auto& notification : List)
		{
			// A delayed toast takes no slot until its turn, otherwise the stack holds a gap open
			// for something the user cannot see yet.
			if (time < notification.Time)
				continue;

			const vec2 text = ImGui::CalcTextSize(notification.Message.c_str());
			const vec2 size = { std::max(text.x + padding * 2 + accent, font * MinWidthScale), text.y + padding * 2 };

			if (notification.Slot < 0.0f)
				notification.Slot = target;
			else
				notification.Slot += (target - notification.Slot) * std::clamp(delta * StackSpeed, 0.0f, 1.0f);

			const float alpha = Fade(notification, time);
			const float offset = (1.0f - alpha) * (size.x + margin * 2);

			const vec2 topLeft = { UI::Screen.RealMax.x - margin - size.x + offset, UI::Screen.RealMin.y
					+ notification.Slot };
			const vec2 bottomRight = topLeft + size;

			draw->AddRectFilled(topLeft, bottomRight, WithAlpha(NotificationBackground, alpha), rounding);
			draw->AddRect(topLeft, bottomRight, WithAlpha(NotificationBorder, alpha), rounding);
			draw->AddRectFilled({ topLeft.x, topLeft.y }, { topLeft.x + accent, bottomRight.y },
				WithAlpha(Accent(notification.Level), alpha), rounding, ImDrawFlags_RoundCornersLeft);
			draw->AddText({ topLeft.x + accent + padding, topLeft.y + padding },
				WithAlpha(NotificationText, alpha), notification.Message.c_str());

			target += size.y + spacing;
		}
		std::erase_if(List, [time](const Notification& notification)
			{ return time > notification.Time + notification.Duration; });
	}

	ImU32 Notifications::Accent(NotificationLevel level)
	{
		switch (level)
		{
		case NotificationLevel::Success:
			return IM_COL32(64, 214, 122, 255);
		case NotificationLevel::Warning:
			return IM_COL32(255, 176, 46, 255);
		case NotificationLevel::Error:
			return IM_COL32(238, 78, 78, 255);
		default:
			return IM_COL32(140, 20, 252, 255);
		}
	}

	// Slide in, hold, slide out. Whichever end of the lifetime is closer owns the value, so a toast
	// pushed with a duration shorter than two slides still fades all the way out.
	float Notifications::Fade(const Notification& notification, double time)
	{
		const auto elapsed = static_cast<float>(time - notification.Time);
		const float remaining = notification.Duration - elapsed;

		return std::min(EaseOut(elapsed / SlideTime), EaseOut(remaining / SlideTime));
	}
}
