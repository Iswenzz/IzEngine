#pragma once
#include "Engine/Core/Base.hpp"

namespace IzEngine
{
	struct HTTPResponse
	{
		bool Success = false;
		long Code = 0;
		std::string Error;
		std::string Body;
		std::unordered_map<std::string, std::string> Headers;
	};

	class API HTTPRequest
	{
	public:
		std::string URL;
		std::string Method;
		std::string Body;
		std::unordered_map<std::string, std::string> Headers;
		std::function<void(const HTTPResponse&)> Callback;
		std::function<void(float)> OnProgress;
		std::function<bool(const char* data, size_t size)> OnData;

		long TimeoutSeconds = 30;
		long ConnectTimeoutSeconds = 10;
		long LowSpeedLimitBytes = 0;
		long LowSpeedTimeSeconds = 0;
		long BufferSizeBytes = 0;

		void Send() const;
	};

	class API HTTP
	{
	public:
		static void Initialize();
		static void Shutdown();

		static void SetCABundle(const std::string& path);
		static const std::string& GetCABundle();
		static void ApplyTLS(void* curlHandle);

		static HTTPRequest Get(const std::string& url, std::function<void(const HTTPResponse&)> callback);
		static HTTPRequest Post(const std::string& url, const std::string& body,
			std::function<void(const HTTPResponse&)> callback);
	};
}
