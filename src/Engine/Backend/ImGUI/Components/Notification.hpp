#pragma once
#include "Engine/Backend/ImGUI/Base.hpp"

namespace IzEngine
{
	class API Notifications
	{
	private:
		struct Notification
		{
			std::string message;
			double time;
		};

	public:
		static inline std::vector<Notification> List;

		static void Push(const std::string& msg);
		static void Render();
	};
}
