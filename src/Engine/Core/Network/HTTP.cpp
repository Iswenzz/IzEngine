#include "HTTP.hpp"

#include "Engine/Core/System/ThreadPool.hpp"

#include <curl/curl.h>

static size_t WriteCallback(char* ptr, size_t size, size_t nmemb, std::string* data)
{
	data->append(ptr, size * nmemb);
	return size * nmemb;
}

static int ProgressCallback(void* clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t, curl_off_t)
{
	if (dltotal > 0)
	{
		auto* cb = reinterpret_cast<std::function<void(float)>*>(clientp);
		(*cb)(static_cast<float>(dlnow) / static_cast<float>(dltotal));
	}
	return 0;
}

namespace IzEngine
{
	void HTTP::Initialize()
	{
		curl_global_init(CURL_GLOBAL_ALL);
	}

	void HTTP::Shutdown()
	{
		curl_global_cleanup();
	}

	void HTTPRequest::Send()
	{
		ThreadPool::Queue(
			[this]()
			{
				CURL* curl = curl_easy_init();
				if (!curl)
					return;

				HTTPResponse response;
				curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());
				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
				curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response.Body);

				curl_slist* headers = nullptr;
				for (const auto& [key, value] : Headers)
					headers = curl_slist_append(headers, (key + ": " + value).c_str());
				if (headers)
					curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

				if (Method == "POST")
				{
					curl_easy_setopt(curl, CURLOPT_POST, 1L);
					curl_easy_setopt(curl, CURLOPT_POSTFIELDS, Body.c_str());
				}
				if (OnProgress)
				{
					curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
					curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, ProgressCallback);
					curl_easy_setopt(curl, CURLOPT_XFERINFODATA, &OnProgress);
				}
				curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
				curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
				curl_easy_perform(curl);
				curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response.Code);

				if (headers)
					curl_slist_free_all(headers);
				curl_easy_cleanup(curl);

				if (Callback)
					Callback(response);
			});
	}

	HTTPRequest* HTTP::Get(const std::string& url, std::function<void(const HTTPResponse&)> callback)
	{
		auto* request = new HTTPRequest();
		request->URL = url;
		request->Method = "GET";
		request->Callback = callback;
		return request;
	}

	HTTPRequest* HTTP::Post(const std::string& url, const std::string& body,
		std::function<void(const HTTPResponse&)> callback)
	{
		auto* request = new HTTPRequest();
		request->URL = url;
		request->Method = "POST";
		request->Body = body;
		request->Callback = callback;
		return request;
	}
}
