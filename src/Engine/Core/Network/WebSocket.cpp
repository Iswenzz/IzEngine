#include "WebSocket.hpp"

#include "HTTP.hpp"

#include <curl/curl.h>

#ifndef _WIN32
	#include <sys/select.h>
#endif

namespace IzEngine
{
	static void WaitOnSocket(curl_socket_t sockfd, long timeoutMs)
	{
		if (sockfd == CURL_SOCKET_BAD)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(timeoutMs));
			return;
		}
		fd_set readfds;
		FD_ZERO(&readfds);
		FD_SET(sockfd, &readfds);

		timeval tv{};
		tv.tv_sec = timeoutMs / 1000;
		tv.tv_usec = (timeoutMs % 1000) * 1000;

		select(static_cast<int>(sockfd) + 1, &readfds, nullptr, nullptr, &tv);
	}

	static bool SendAll(CURL* curl, const char* data, size_t length, unsigned int flags)
	{
		size_t offset = 0;
		do
		{
			size_t sent = 0;
			const CURLcode result = curl_ws_send(curl, data + offset, length - offset, &sent, 0, flags);
			offset += sent;

			if (result == CURLE_AGAIN)
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			else if (result != CURLE_OK)
				return false;
		} while (offset < length);
		return true;
	}

	WebSocket::~WebSocket()
	{
		Disconnect();
	}

	void WebSocket::Connect(const std::string& url)
	{
		Disconnect();

		CloseRequested = false;
		{
			std::lock_guard<std::mutex> lock(SendMutex);
			SendQueue.clear();
		}
		Thread = std::thread(&WebSocket::Run, this, url);
	}

	void WebSocket::Disconnect()
	{
		CloseRequested = true;
		if (Thread.joinable())
			Thread.join();
	}

	void WebSocket::Run(std::string url)
	{
		CURL* curl = curl_easy_init();

		if (!curl)
		{
			if (OnError)
				OnError("curl_easy_init failed");
			return;
		}
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 2L);
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);
		curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);

		HTTP::ApplyTLS(curl);

		const CURLcode connectResult = curl_easy_perform(curl);
		if (connectResult != CURLE_OK)
		{
			curl_easy_cleanup(curl);
			if (OnError)
				OnError(curl_easy_strerror(connectResult));
			if (OnClose)
				OnClose();
			return;
		}
		curl_socket_t sockfd = CURL_SOCKET_BAD;
		curl_easy_getinfo(curl, CURLINFO_ACTIVESOCKET, &sockfd);

		Open = true;
		if (OnOpen)
			OnOpen();

		std::string messageBuffer;
		bool messageIsBinary = false;
		bool sendCloseFrame = true;

		while (!CloseRequested)
		{
			std::deque<std::pair<std::string, bool>> outgoing;
			{
				std::lock_guard<std::mutex> lock(SendMutex);
				outgoing.swap(SendQueue);
			}
			bool sendFailed = false;
			for (const auto& [payload, isBinary] : outgoing)
			{
				if (!SendAll(curl, payload.data(), payload.size(), isBinary ? CURLWS_BINARY : CURLWS_TEXT))
				{
					sendFailed = true;
					break;
				}
			}
			if (sendFailed)
			{
				if (OnError)
					OnError("WebSocket send failed");
				sendCloseFrame = false;
				break;
			}
			char buffer[4096];
			size_t received = 0;
			const curl_ws_frame* frame = nullptr;
			const CURLcode result = curl_ws_recv(curl, buffer, sizeof(buffer), &received, &frame);

			if (result == CURLE_AGAIN)
			{
				WaitOnSocket(sockfd, 50);
				continue;
			}
			if (result != CURLE_OK || !frame)
			{
				if (OnError && result != CURLE_OK)
					OnError(curl_easy_strerror(result));
				sendCloseFrame = false;
				break;
			}
			if (frame->flags & CURLWS_CLOSE)
			{
				sendCloseFrame = false;
				break;
			}
			if (frame->flags & CURLWS_PING)
				continue;

			if (messageBuffer.empty())
				messageIsBinary = (frame->flags & CURLWS_BINARY) != 0;

			messageBuffer.append(buffer, received);
			const bool moreFragments = (frame->flags & CURLWS_CONT) != 0 || frame->bytesleft > 0;

			if (!moreFragments)
			{
				if (messageIsBinary && OnBinary)
					OnBinary(messageBuffer);
				else if (OnMessage)
					OnMessage(messageBuffer);
				messageBuffer.clear();
			}
		}
		if (sendCloseFrame)
		{
			size_t sent = 0;
			curl_ws_send(curl, "", 0, &sent, 0, CURLWS_CLOSE);
		}
		Open = false;
		curl_easy_cleanup(curl);

		if (OnClose)
			OnClose();
	}

	void WebSocket::Send(const std::string& message)
	{
		if (!Open)
			return;

		std::lock_guard<std::mutex> lock(SendMutex);
		SendQueue.emplace_back(message, false);
	}

	void WebSocket::SendBinary(const std::string& data)
	{
		if (!Open)
			return;

		std::lock_guard<std::mutex> lock(SendMutex);
		SendQueue.emplace_back(data, true);
	}

	void WebSocket::Close()
	{
		CloseRequested = true;
	}

	bool WebSocket::IsOpen() const
	{
		return Open;
	}
}
