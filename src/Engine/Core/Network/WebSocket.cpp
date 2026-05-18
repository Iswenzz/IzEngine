#include "WebSocket.hpp"

#include "Engine/Core/System/ThreadPool.hpp"

#include <curl/curl.h>

namespace IzEngine
{
	void WebSocket::Connect(const std::string& url)
	{
		ThreadPool::Queue(
			[this, url]()
			{
				CURL* curl = curl_easy_init();
				if (!curl)
					return;

				curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
				curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 2L);

				CURLcode res = curl_easy_perform(curl);
				if (res != CURLE_OK)
				{
					curl_easy_cleanup(curl);
					return;
				}
				Handle = curl;
				Open = true;

				if (OnOpen)
					OnOpen();

				while (Open)
				{
					const curl_ws_frame* frame = nullptr;
					char buffer[4096];
					size_t received = 0;

					res = curl_ws_recv(curl, buffer, sizeof(buffer), &received, &frame);
					if (res == CURLE_OK && received > 0)
					{
						if (OnMessage)
							OnMessage(std::string(buffer, received));
					}
					else if (res != CURLE_AGAIN)
						break;
				}
				curl_easy_cleanup(curl);
				Handle = nullptr;
				Open = false;

				if (OnClose)
					OnClose();
			});
	}

	void WebSocket::Send(const std::string& message)
	{
		if (!Handle || !Open)
			return;

		size_t sent = 0;
		curl_ws_send(reinterpret_cast<CURL*>(Handle), message.c_str(), message.size(), &sent, 0, CURLWS_TEXT);
	}

	void WebSocket::Close()
	{
		Open = false;
	}

	bool WebSocket::IsOpen() const
	{
		return Open;
	}
}
