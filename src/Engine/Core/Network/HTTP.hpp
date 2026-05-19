#pragma once
#include "Engine/Core/Base.hpp"

namespace IzEngine
{
	struct HTTPResponse
	{
		int Code = 0;
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

		void Send();
	};

	class API HTTP
	{
	public:
		static void Initialize();
		static void Shutdown();

		static HTTPRequest* Get(const std::string& url, std::function<void(const HTTPResponse&)> callback);
		static HTTPRequest* Post(const std::string& url, const std::string& body,
			std::function<void(const HTTPResponse&)> callback);
	};
}
