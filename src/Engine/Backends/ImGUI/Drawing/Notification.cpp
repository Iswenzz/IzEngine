#include "Notification.hpp"

namespace IW3SR
{
	NotificationCenter::NotificationCenter(const std::string& msg)
	{
		notifications.push_back({ msg, 5 });
	}

	NotificationCenter::NotificationCenter(const std::string& msg, int duration)
	{
		notifications.push_back({ msg, duration });
	}

	void NotificationCenter::Create(const std::string& msg, int duration)
	{
		notifications.push_back({ msg, duration });
	}

	void NotificationCenter::Render()
	{
		if (notifications.empty())
			return;

		ImGui::SetNextWindowPos(ImVec2{ 0, 540 }, ImGuiCond_Always);
		ImGui::Begin("Notification", nullptr, ImGuiWindowFlags_Notification);

		for (const auto& notification : notifications)
			ImGui::Text(std::format("IW3SR: {}", notification.message).c_str());

		ImGui::End();

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
