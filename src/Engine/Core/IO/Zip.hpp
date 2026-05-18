#pragma once
#include "File.hpp"

namespace IzEngine
{
	class API Zip
	{
	public:
		static bool Extract(const std::filesystem::path& zipPath, const std::filesystem::path& destDir);
		static bool Compress(const std::filesystem::path& sourceDir, const std::filesystem::path& zipPath);
		static bool Add(const std::filesystem::path& zipPath, const File& file);
		static bool Remove(const std::filesystem::path& zipPath, const std::string& entryPath);
		static bool Contains(const std::filesystem::path& zipPath, const std::string& entryPath);
		static File Read(const std::filesystem::path& zipPath, const std::string& entryPath);
	};
}
