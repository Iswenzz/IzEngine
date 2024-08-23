#include "Notification.hpp"

#include "ImGUI/UI.hpp"

namespace IzEngine
{
	void Notifications::Push(const std::string& msg, float duration)
	{
		List.push_back({ msg, duration, UI::Get().Time() });
	}

	void Notifications::Render()
	{
		if (List.empty())
			return;

		ImDrawList* draw = ImGui::GetBackgroundDrawList();

		int position = 50;
		int slideDuration = 1;
		double currentTime = UI::Get().Time();

		for (const auto& notification : List)
		{
			float elapsedTime = currentTime - notification.time;
			int slidePosition = position + size.y;

			if (elapsedTime < slideDuration)
				slidePosition = static_cast<int>((elapsedTime / slideDuration) * slidePosition);

			Window window(UUID().String);
			window.SetRect(0, slidePosition, 140, 20);
			window.Begin(ImGuiWindowFlags_Notification);

			const ImVec2& pos = window.RenderPosition;
			const ImVec2& size = window.RenderSize;

			draw->AddRectFilled({ -1, pos.y }, { pos.x + size.x, pos.y + size.y }, IM_COL32(0, 0, 0, 255));
			draw->AddRectFilled({ pos.x + size.x, pos.y }, { pos.x + size.x + 5, pos.y + size.y },
				IM_COL32(140, 20, 252, 255));

			ImGui::TextWrapped(notification.message.c_str());
			window.End();

			position += size.y;
		}
		std::erase_if(List, [](const Notification& notification)
			{ return UI::Get().Time() > notification.time + notification.duration; });
	}
}
