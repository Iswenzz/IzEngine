#include "VFS.hpp"

#include "Engine/Core/Utils/StringUtils.hpp"

#include <libzippp/libzippp.h>

using namespace libzippp;

namespace IzEngine
{
	void VFS::Index(const std::string& directory, const std::string& extension)
	{
		for (const auto& entry : std::filesystem::recursive_directory_iterator(directory))
		{
			if (!entry.is_regular_file())
				continue;

			const auto& path = entry.path();
			if (path.extension() != extension)
				continue;

			const std::string archivePath = StringUtils::NormalizePath(path.string());
			ZipArchive archive(archivePath);
			if (!archive.open(ZipArchive::ReadOnly))
				continue;

			std::filesystem::path relativeDir = std::filesystem::relative(path.parent_path(), directory);
			std::vector<ZipEntry> entries = archive.getEntries();

			for (const auto& zipEntry : entries)
			{
				if (zipEntry.isDirectory())
					continue;

				std::string key;
				if (relativeDir == ".")
					key = StringUtils::NormalizePath(zipEntry.getName());
				else
					key = StringUtils::NormalizePath((relativeDir / zipEntry.getName()).string());

				ArchiveFileInfo value{ archivePath, zipEntry.getName() };
				Tree.emplace(std::move(key), std::move(value));
			}
			archive.close();
		}
	}

	File VFS::GetFile(const std::string& filePath)
	{
		File file;
		file.Path = filePath;

		auto it = Tree.find(StringUtils::NormalizePath(filePath));
		if (it == Tree.end())
			return file;

		const ArchiveFileInfo& info = it->second;
		ZipArchive archive(info.archivePath);
		if (!archive.open(ZipArchive::ReadOnly))
			return file;

		ZipEntry entry = archive.getEntry(info.entryPath);
		if (entry.isNull())
		{
			archive.close();
			return file;
		}
		auto size = entry.getSize();
		void* data = entry.readAsBinary();

		if (!data)
		{
			archive.close();
			return file;
		}
		file.Data = std::vector<uint8_t>(static_cast<uint8_t*>(data), static_cast<uint8_t*>(data) + size);
		free(data);
		archive.close();
		return file;
	}

	std::optional<ArchiveFileInfo> VFS::GetFileEntry(const std::string& filePath)
	{
		auto it = Tree.find(StringUtils::NormalizePath(filePath));
		return it == Tree.end() ? std::nullopt : std::optional<ArchiveFileInfo>(it->second);
	}
}
