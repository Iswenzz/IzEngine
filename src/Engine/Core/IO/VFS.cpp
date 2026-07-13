#include "VFS.hpp"

#include "Engine/Core/Utils/StringUtils.hpp"

#include <libzippp/libzippp.h>

using namespace libzippp;

namespace IzEngine
{
	void VFS::Index(const IndexDescriptor& desc)
	{
		for (const auto& entry : std::filesystem::recursive_directory_iterator(desc.Directory))
		{
			if (!entry.is_regular_file())
				continue;

			const auto& path = entry.path();
			const bool isArchive = !desc.ArchiveExtension.empty() && path.extension() == desc.ArchiveExtension;

			auto relativeDir = std::filesystem::relative(path.parent_path(), desc.Directory);
			if (relativeDir == ".")
				relativeDir.clear();

			if (!desc.Patterns.empty()
				&& !std::ranges::any_of(desc.Patterns, [&](const auto& p)
					{ return StringUtils::NormalizePath(path.string()).contains(StringUtils::NormalizePath(p)); }))
				continue;

			if (isArchive)
			{
				const std::string archivePath = StringUtils::NormalizePath(path.string());

				ZipArchive archive(archivePath);
				if (!archive.open(ZipArchive::ReadOnly))
					continue;
				for (const auto& zipEntry : archive.getEntries())
				{
					if (zipEntry.isDirectory())
						continue;

					std::string key = StringUtils::NormalizePath((relativeDir / zipEntry.getName()).string());
					FileInfo value{ archivePath, zipEntry.getName() };

					Tree.emplace(std::move(key), std::move(value));
				}
				archive.close();
				continue;
			}
			std::string key = StringUtils::NormalizePath((relativeDir / path.filename()).string());
			FileInfo value{ "", StringUtils::NormalizePath(path.string()) };
			Tree.emplace(std::move(key), std::move(value));
		}
	}

	File VFS::GetFile(const std::string& filePath)
	{
		File file;
		file.Path = filePath;

		auto it = Tree.find(StringUtils::NormalizePath(filePath));
		if (it == Tree.end())
			return file;

		const FileInfo& info = it->second;

		if (info.ArchivePath.empty())
		{
			std::ifstream stream(info.SourcePath, std::ios::binary);
			if (!stream)
				return file;

			file.Data.assign(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
			return file;
		}
		ZipArchive archive(info.ArchivePath);
		if (!archive.open(ZipArchive::ReadOnly))
			return file;

		ZipEntry entry = archive.getEntry(info.SourcePath);
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

	File VFS::GetFirstFile(const std::string& query)
	{
		const std::string normalizedQuery = StringUtils::NormalizePath(query);

		for (const auto& [key, info] : Tree)
		{
			if (key.find(normalizedQuery) != std::string::npos)
				return GetFile(key);
		}
		return {};
	}

	std::optional<FileInfo> VFS::GetFileEntry(const std::string& filePath)
	{
		auto it = Tree.find(StringUtils::NormalizePath(filePath));
		return it == Tree.end() ? std::nullopt : std::optional<FileInfo>(it->second);
	}

	std::vector<std::pair<std::string, FileInfo>> VFS::SearchFiles(const std::string& query)
	{
		std::vector<std::pair<std::string, FileInfo>> results;
		const std::string normalizedQuery = StringUtils::NormalizePath(query);

		for (const auto& [key, info] : Tree)
		{
			if (key.find(normalizedQuery) != std::string::npos)
				results.emplace_back(key, info);
		}
		return results;
	}
}
