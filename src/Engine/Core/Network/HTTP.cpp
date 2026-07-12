#include "HTTP.hpp"

#include "Engine/Core/System/ThreadPool.hpp"

#include <curl/curl.h>

static std::string CABundlePath;

static size_t WriteCallback(char* ptr, size_t size, size_t nmemb, std::string* data)
{
	data->append(ptr, size * nmemb);
	return size * nmemb;
}

static size_t HeaderCallback(char* buffer, size_t size, size_t nitems,
	std::unordered_map<std::string, std::string>* headers)
{
	const size_t total = size * nitems;
	std::string line(buffer, total);

	const size_t colon = line.find(':');
	if (colon != std::string::npos)
	{
		std::string key = line.substr(0, colon);
		const size_t start = line.find_first_not_of(" \t", colon + 1);
		const size_t end = line.find_last_not_of("\r\n \t");
		std::string value = (start != std::string::npos && end != std::string::npos && end >= start)
			? line.substr(start, end - start + 1)
			: std::string();
		(*headers)[std::move(key)] = std::move(value);
	}
	return total;
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

	void HTTP::SetCABundle(const std::string& path)
	{
		CABundlePath = path;
	}

	const std::string& HTTP::GetCABundle()
	{
		return CABundlePath;
	}

	void HTTP::ApplyTLS(void* curlHandle)
	{
		CURL* curl = static_cast<CURL*>(curlHandle);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);

		if (!CABundlePath.empty())
		{
			curl_easy_setopt(curl, CURLOPT_CAINFO, CABundlePath.c_str());
		}
		else
		{
#ifdef CURLSSLOPT_NATIVE_CA
			curl_easy_setopt(curl, CURLOPT_SSL_OPTIONS, static_cast<long>(CURLSSLOPT_NATIVE_CA));
#endif
		}
	}

	void HTTPRequest::Send() const
	{
		ThreadPool::Queue(
			[request = *this]() mutable
			{
				HTTPResponse response;

				CURL* curl = curl_easy_init();
				if (!curl)
				{
					response.Error = "curl_easy_init failed";
					if (request.Callback)
						request.Callback(response);
					return;
				}
				curl_easy_setopt(curl, CURLOPT_URL, request.URL.c_str());
				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
				curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response.Body);
				curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, HeaderCallback);
				curl_easy_setopt(curl, CURLOPT_HEADERDATA, &response.Headers);
				curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
				curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 10L);
				curl_easy_setopt(curl, CURLOPT_TIMEOUT, request.TimeoutSeconds);
				curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, request.ConnectTimeoutSeconds);
				curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);

				HTTP::ApplyTLS(curl);

				curl_slist* headers = nullptr;
				for (const auto& [key, value] : request.Headers)
					headers = curl_slist_append(headers, (key + ": " + value).c_str());
				if (headers)
					curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

				if (request.Method == "POST")
					curl_easy_setopt(curl, CURLOPT_POST, 1L);
				else if (request.Method != "GET")
					curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, request.Method.c_str());

				if (!request.Body.empty() || request.Method == "POST")
				{
					curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request.Body.c_str());
					curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, static_cast<long>(request.Body.size()));
				}
				if (request.OnProgress)
				{
					curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
					curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, ProgressCallback);
					curl_easy_setopt(curl, CURLOPT_XFERINFODATA, &request.OnProgress);
				}
				const CURLcode result = curl_easy_perform(curl);
				response.Success = (result == CURLE_OK);
				if (result != CURLE_OK)
					response.Error = curl_easy_strerror(result);

				curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response.Code);

				if (headers)
					curl_slist_free_all(headers);

				curl_easy_cleanup(curl);

				if (request.Callback)
					request.Callback(response);
			});
	}

	HTTPRequest HTTP::Get(const std::string& url, std::function<void(const HTTPResponse&)> callback)
	{
		HTTPRequest request;
		request.URL = url;
		request.Method = "GET";
		request.Callback = std::move(callback);
		return request;
	}

	HTTPRequest HTTP::Post(const std::string& url, const std::string& body,
		std::function<void(const HTTPResponse&)> callback)
	{
		HTTPRequest request;
		request.URL = url;
		request.Method = "POST";
		request.Body = body;
		request.Callback = std::move(callback);
		return request;
	}
}
