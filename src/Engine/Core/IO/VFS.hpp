#pragma once
#include "File.hpp"

namespace IzEngine
{
	struct ArchiveFileInfo
	{
		std::string archivePath;
		std::string entryPath;
	};

	class API VFS
	{
	public:
		static inline std::unordered_map<std::string, ArchiveFileInfo> Tree;

		static void Index(const std::string& directory, const std::string& extension);
		static File GetFile(const std::string& filePath);
		static std::optional<ArchiveFileInfo> GetFileEntry(const std::string& filePath);
	};
}
