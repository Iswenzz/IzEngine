#include "Notification.hpp"

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
			if (!count)
				ImGui::SetNextWindowPos({ 0, 300 });
			else
				ImGui::SetNextWindowPos({ 0, next });

			ImGui::SetNextWindowSize({ 300, 50 });
			ImGui::Begin(std::format("Notification # {}", count).c_str(), nullptr, ImGuiWindowFlags_Notification);

			const ImVec2 pos = ImGui::GetWindowPos();
			const ImVec2 size = ImGui::GetWindowSize();

			draw->AddRectFilled({ -1, pos.y }, { pos.x + size.x, pos.y + size.y }, IM_COL32(0, 0, 0, 255));
			draw->AddRectFilled({ pos.x + size.x, pos.y }, { pos.x + size.x + 5, pos.y + size.y }, IM_COL32(140, 20, 252, 255));

			const std::string index = std::format("IW3SR: {}", notification.message);

			ImGui::SetCursorPosX(size.x / 2 - ImGui::CalcTextSize(index.c_str()).x / 2);
			ImGui::SetCursorPosY(size.y / 2 - ImGui::CalcTextSize(index.c_str()).y / 2);
			ImGui::Text(index.c_str());

			next = pos.y + size.y + 10;
			count++;
			ImGui::End();
		}
		Notifications.erase(std::remove_if(Notifications.begin(), Notifications.end(),
			[](const Notification& notification)
			{
				const Seconds duration(notification.duration);
				const auto currentTime = Time::now();
				const static auto endTime = currentTime + duration;
				return currentTime > endTime;
			}), Notifications.end());
	}
}
