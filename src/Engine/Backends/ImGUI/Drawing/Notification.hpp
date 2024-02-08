#pragma once
#include "Engine/Backends/ImGUI/Drawing/Window.hpp"

#include <queue>

namespace IW3SR
{
	class API NotificationCenter
	{
	private:
		using MilliSeconds = std::chrono::milliseconds;
		using Seconds = std::chrono::seconds;
		using Time = std::chrono::steady_clock;

		struct Notification
		{
			std::string message;
			int duration;
			//vec4 Color;
			//vec2 Position;
		};
	public:
		std::vector<Notification> notifications;

		NotificationCenter() = default;
		NotificationCenter(const std::string& msg);
		~NotificationCenter() = default;

		/// <summary>
		/// Create a notification.
		/// </summary>
		void Push(const std::string& msg, int duration = 5);

		/// <summary>
		/// Render the notification.
		/// </summary>
		void Render();
	};

	
}
