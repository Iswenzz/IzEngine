#pragma once
#include "Engine/Backends/ImGUI/Drawing/Window.hpp"

#include <queue>

namespace IW3SR
{
	class API NotificationCenter
	{
	private:
		struct Notification
		{
			std::string message;
			int duration;
			//vec4 Color;
			//vec2 Position;
		};

		using Seconds = std::chrono::seconds;
		using Time = std::chrono::steady_clock;
	public:
		std::vector<Notification> notifications;

		/// <summary>
		/// Initialize the notification.
		/// </summary>
		NotificationCenter() = default;
		NotificationCenter(const std::string& msg);
		NotificationCenter(const std::string& msg, int duration);

		/// <summary>
		/// Create a notification.
		/// </summary>
		void Create(const std::string& msg, int duration = 5);

		/// <summary>
		/// Render the notification.
		/// </summary>
		void Render();
	};

	
}
