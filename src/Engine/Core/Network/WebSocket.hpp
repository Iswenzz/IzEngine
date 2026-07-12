#pragma once
#include "Engine/Core/Base.hpp"

namespace IzEngine
{
	class API WebSocket
	{
	public:
		WebSocket() = default;
		WebSocket(const WebSocket&) = delete;
		WebSocket& operator=(const WebSocket&) = delete;
		~WebSocket();

		std::function<void()> OnOpen;
		std::function<void(const std::string&)> OnMessage;
		std::function<void(const std::string&)> OnBinary;
		std::function<void(const std::string&)> OnError;
		std::function<void()> OnClose;

		void Connect(const std::string& url);
		void Close();

		void Send(const std::string& message);
		void SendBinary(const std::string& data);

		bool IsOpen() const;

	private:
		void Run(std::string url);
		void Disconnect();

		std::thread Thread;
		std::atomic<bool> Open{ false };
		std::atomic<bool> CloseRequested{ false };

		std::mutex SendMutex;
		std::deque<std::pair<std::string, bool>> SendQueue;
	};
}
