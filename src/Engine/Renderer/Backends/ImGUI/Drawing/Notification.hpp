#pragma once
#include "ImGUI/Base.hpp"

namespace IzEngine
{
	/// <summary>
	/// Notification center.
	/// </summary>
	class API Notifications
	{
	private:
		/// <summary>
		/// Notification data.
		/// </summary>
		struct Notification
		{
			std::string message;
			float duration;
			double time;
		};

	public:
		static inline std::vector<Notification> List;

		/// <summary>
		/// Create a notification.
		/// </summary>
		/// <param name="msg">The message.</param>
		/// <param name="duration">The duration.</param>
		static void Push(const std::string& msg, float duration = 5);

		/// <summary>
		/// Render the notifications.
		/// </summary>
		static void Render();
	};
}
