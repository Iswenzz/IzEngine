#include "Notification.hpp"

#include "ImGUI/UI.hpp"

namespace IzEngine
{
	void Notifications::Push(const std::string& msg)
	{
		List.push_back({ msg, UI::Time() });
	}

	void Notifications::Render()
	{
		if (List.empty())
			return;

		ImDrawList* draw = ImGui::GetBackgroundDrawList();
		const auto time = UI::Time();
		const float duration = 3.0f;
		const float slideDuration = 0.3f;
		int y = 100;

		for (const auto& notification : List)
		{
			const float elapsed = time - notification.time;
			float x = 0.0f;

			if (elapsed < slideDuration)
			{
				const float t = elapsed / slideDuration;
				x = -150.0f * (1.0f - t);
			}
			else if (elapsed > duration - slideDuration)
			{
				const float t = (elapsed - (duration - slideDuration)) / slideDuration;
				x = (-150.0f * t);
			}
			else
				x = 0.0f;

			Window window;
			window.SetRect(x, y, ImGui::CalcTextSize(notification.message.c_str()).x / 2,
				ImGui::CalcTextSize(notification.message.c_str()).y/1.3f);
			window.SetFlags(ImGuiWindowFlags_Notification);
			window.Begin();

			const ImVec2& pos = window.RenderPosition;
			const ImVec2& size = window.RenderSize;
			const float accentWidth = 8.f;
			const ImVec2 accentMin = ImVec2(pos.x + size.x - accentWidth, pos.y);
			const ImVec2 accentMax = ImVec2(pos.x + size.x, pos.y + size.y);

			draw->AddRectFilled(pos, pos + size, IM_COL32(10, 10, 10, 240));
			draw->AddText(pos + ImVec2(10, 5), IM_COL32(255, 255, 255, 255), notification.message.c_str());
			draw->AddRectFilled(accentMin, accentMax, IM_COL32(255, 100, 255, 255));

			const float progress = ImClamp(1.0f - (elapsed / (duration - slideDuration)), 0.0f, 1.0f);
			const ImVec2 barStart = ImVec2(pos.x, pos.y + size.y - 4);
			const ImVec2 barEnd = ImVec2(pos.x + size.x * progress, pos.y + size.y);
			const ImU32 colLeft = IM_COL32(14, 2, 25, 255);
			const ImU32 colRight = IM_COL32(255, 100, 255, 255);

			draw->AddRectFilledMultiColor(barStart, barEnd,
				colLeft,
				colRight,
				colRight,
				colLeft
			);

			window.End();
			y += window.Size.y + 8;
		}
		List.erase(std::remove_if(List.begin(), List.end(),
					   [&](const Notification& notification) { return time > notification.time + duration; }),
			List.end());
		if (List.size() > 5)
			List.erase(List.begin(), List.end());
	}
}
