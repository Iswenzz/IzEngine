#include "Notification.hpp"
#include "ImGUI/Drawing/Window.hpp"

namespace IW3SR::Engine
{
	void NotificationCenter::Push(const std::string& msg, int duration)
	{
		Notifications.push_back({ msg, duration });
	}

	void NotificationCenter::Render()
	{
		if (Notifications.empty())
			return;

		ImDrawList* draw = ImGui::GetBackgroundDrawList();
		int count = 0;
		float next = 0;

		for (const auto& notification : Notifications)
		{
			Window window;
			window.SetRect(0, count ? next : 50, 80, 20);
			window.Begin(ImGuiWindowFlags_Notification);

			const ImVec2& pos = window.RenderPosition;
			const ImVec2& size = window.RenderSize;

			draw->AddRectFilled({ -1, pos.y }, { pos.x + size.x, pos.y + size.y }, IM_COL32(0, 0, 0, 255));
			draw->AddRectFilled({ pos.x + size.x, pos.y }, { pos.x + size.x + 5, pos.y + size.y }, IM_COL32(140, 20, 252, 255));

			const std::string message = std::format("IW3SR: {}", notification.message);
			ImGui::SetCursorPos(size / 2 - ImGui::CalcTextSize(message.c_str()) / 2);
			ImGui::Text(message.c_str());
			window.End();

			next = pos.y + size.y + 10;
			count++;
		}
		Notifications.erase(std::remove_if(Notifications.begin(), Notifications.end(), [](const Notification& notification)
		{
			const Seconds duration(notification.duration);
			const auto currentTime = Time::now();
			const static auto endTime = currentTime + duration;
			return currentTime > endTime;
		}), Notifications.end());
	}
}
