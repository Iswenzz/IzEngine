#pragma once
#include "File.hpp"

namespace IzEngine
{
	struct FileInfo
	{
		std::string ArchivePath;
		std::string SourcePath;
	};

	struct IndexDescriptor
	{
		std::string Directory;
		std::string ArchiveExtension;
		std::vector<std::string> Patterns;
	};

	class VFS
	{
	public:
		static inline std::unordered_map<std::string, FileInfo> Tree;

		static void Index(const IndexDescriptor& desc);
		static File GetFile(const std::string& filePath);
		static File GetFirstFile(const std::string& query);
		static std::optional<FileInfo> GetFileEntry(const std::string& filePath);
		static std::vector<std::pair<std::string, FileInfo>> SearchFiles(const std::string& query);
	};
}
