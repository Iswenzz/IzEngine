#pragma once
#include "ImGUI/Components.hpp"

namespace IzEngine
{
	/// <summary>
	/// Notification center.
	/// </summary>
	class API NotificationCenter
	{
	private:
		struct Notification
		{
			std::string message;
			int duration;
		};

	public:
		std::vector<Notification> Notifications;

		/// <summary>
		/// Initialize the notification center.
		/// </summary>
		NotificationCenter() = default;
		~NotificationCenter() = default;

		/// <summary>
		/// Create a notification.
		/// </summary>
		/// <param name="msg">The message.</param>
		/// <param name="duration">The duration.</param>
		void Push(const std::string& msg, int duration = 5);

		/// <summary>
		/// Render the notifications.
		/// </summary>
		void Render();

	private:
		using MilliSeconds = std::chrono::milliseconds;
		using Seconds = std::chrono::seconds;
		using Time = std::chrono::steady_clock;
	};
}
