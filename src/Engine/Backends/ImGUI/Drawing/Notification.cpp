#include "Notification.hpp"

namespace IW3SR
{
	NotificationCenter::NotificationCenter(const std::string& msg)
	{
		notifications.push_back({ msg, 5 });
	}

	void NotificationCenter::Push(const std::string& msg, int duration)
	{
		notifications.push_back({ msg, duration });
	}

	void NotificationCenter::Render()
	{
		if (notifications.empty())
			return;

		ImDrawList* draw = ImGui::GetBackgroundDrawList();

		int windowCount = 0;
		float nextWindow = 0;
		for (const auto& notification : notifications)
		{
			if (!windowCount)
				ImGui::SetNextWindowPos({ 0, 300 });
			else ImGui::SetNextWindowPos({ 0, nextWindow });

			ImGui::SetNextWindowSize({ 300, 50 }, ImGuiCond_Always);
			ImGui::Begin(("Notification #" + std::to_string(windowCount)).c_str(), nullptr, ImGuiWindowFlags_Notification);

			const ImVec2 pos = ImGui::GetWindowPos();
			const ImVec2 size = ImGui::GetWindowSize();

			draw->AddRectFilled({ -1, pos.y }, { pos.x + size.x, pos.y + size.y }, IM_COL32(0, 0, 0, 255));
			draw->AddRectFilled({ pos.x + size.x, pos.y }, { pos.x + size.x + 5, pos.y + size.y }, IM_COL32(140, 20, 252, 255));

			//ImGui::SameLine(size.x / 2 - ImGui::CalcTextSize(notification.message.c_str()).x);
			ImGui::Text(std::format("IW3SR: {}", notification.message).c_str());

			nextWindow = pos.y + size.y + 10;
			windowCount++;
			ImGui::End();
		}

		notifications.erase(std::remove_if(notifications.begin(), notifications.end(),
			[](const Notification& notification)
			{
				const Seconds duration(notification.duration);
				const auto currentTime = Time::now();
				const static auto endTime = currentTime + duration;
				return currentTime > endTime;
			}), notifications.end());
	}
}
