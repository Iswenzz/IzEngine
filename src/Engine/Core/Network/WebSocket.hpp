#pragma once
#include "Engine/Core/Base.hpp"

namespace IzEngine
{
	class API WebSocket
	{
	public:
		std::function<void()> OnOpen;
		std::function<void(const std::string&)> OnMessage;
		std::function<void()> OnClose;

		void Connect(const std::string& url);
		void Send(const std::string& message);
		void Close();
		bool IsOpen() const;

	private:
		void* Handle = nullptr;
		bool Open = false;
	};
}
