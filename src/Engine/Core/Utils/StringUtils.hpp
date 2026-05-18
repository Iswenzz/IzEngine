#pragma once
#include "Engine/Core/Base.hpp"

namespace IzEngine
{
	class API StringUtils
	{
	public:
		static std::string ToLower(const std::string& str);
		static std::string NormalizePath(const std::string& path);
	};
}
