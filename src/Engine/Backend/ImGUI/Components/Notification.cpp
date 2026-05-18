#include "Notification.hpp"

#include "Engine/Backend/ImGUI/Common.hpp"

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
		const float duration = 4.0f;
		const float slideDuration = 0.3f;
		const float leftTime = 3.7f;
		const float centerTime = 0.3f;
		int y = 100;

		for (const auto& notification : List)
		{
			const float elapsed = time - notification.time;
			int x = 0;

			if (elapsed < centerTime)
				x = std::lerp(-140, 0, elapsed / slideDuration);
			if (elapsed > leftTime)
				x = std::lerp(0, -140, (elapsed - leftTime) / slideDuration);

			Frame frame;
			frame.SetRect(x, y, 140, 20);
			frame.SetFlags(ImGuiWindowFlags_Notification);
			frame.Begin();

			const auto& pos = frame.RenderPosition;
			const auto& size = frame.RenderSize;

			draw->AddRectFilled({ 0, pos.y }, { pos.x + size.x, pos.y + size.y }, IM_COL32(0, 0, 0, 255));
			draw->AddRectFilled({ pos.x + size.x, pos.y }, { pos.x + size.x + 5, pos.y + size.y },
				IM_COL32(140, 20, 252, 255));
			draw->AddText({ pos.x, pos.y + (size.y / 6) }, IM_COL32(255, 255, 255, 255), notification.message.c_str());

			frame.End();
			y += frame.Size.y + 2;
		}
		std::erase_if(List, [&](const Notification& notification) { return time > notification.time + duration; });
	}
}
