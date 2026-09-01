#pragma once
#include "Engine/Backend/ImGUI/Base.hpp"

namespace IzEngine
{
	enum class NotificationLevel
	{
		Info,
		Success,
		Warning,
		Error
	};

	// One toast in the stack. Time is when it starts showing, delay included, so a pending toast is
	// simply one the clock has not reached yet. Slot is the animated distance from the top of the
	// safe area, kept on the entry so the ones below an expiring toast slide up instead of snapping
	// into its place.
	struct Notification
	{
		std::string Message;
		NotificationLevel Level = NotificationLevel::Info;
		double Time = 0;
		float Duration = 4.0f;
		float Slot = -1.0f;
	};

	class API Notifications
	{
	public:
		static inline std::vector<Notification> List;
		static inline size_t Max = 8;

		static void Push(const std::string& message, NotificationLevel level = NotificationLevel::Info,
			float duration = 4.0f, float delay = 0.0f);
		static void Clear();
		static void Render();

	private:
		static ImU32 Accent(NotificationLevel level);
		static float Fade(const Notification& notification, double time);
	};
}
