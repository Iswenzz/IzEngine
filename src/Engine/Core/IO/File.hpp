#pragma once
#include "Engine/Core/Base.hpp"

namespace IzEngine
{
	class API File
	{
	public:
		std::filesystem::path Path;
		std::vector<uint8_t> Data;

		bool IsValid() const;

		static std::string ReadText(const std::filesystem::path& path);
		static File Read(const std::filesystem::path& path);
	};
}
