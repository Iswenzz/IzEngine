#include "Zip.hpp"

#include <libzippp/libzippp.h>

using namespace libzippp;

namespace IzEngine
{
	static bool ResolveEntryPath(const std::filesystem::path& destDir, const std::string& entryName,
		std::filesystem::path& out)
	{
		const std::filesystem::path name(entryName);
		if (name.is_absolute() || name.has_root_name())
			return false;

		const auto root = (destDir / "").lexically_normal();
		const auto dest = (destDir / name).lexically_normal();

		if (dest.native().starts_with(root.native()))
		{
			out = dest;
			return true;
		}
		return false;
	}

	bool Zip::Extract(const std::filesystem::path& zipPath, const std::filesystem::path& destDir)
	{
		ZipArchive archive(zipPath.string());
		if (!archive.open(ZipArchive::ReadOnly))
			return false;

		for (const auto& entry : archive.getEntries())
		{
			if (entry.isDirectory())
				continue;

			std::filesystem::path dest;
			if (!ResolveEntryPath(destDir, entry.getName(), dest))
			{
				Log::WriteLine(Channel::Warning, "Skipping zip entry outside the destination: {}", entry.getName());
				continue;
			}
			std::error_code ec;
			std::filesystem::create_directories(dest.parent_path(), ec);

			void* data = entry.readAsBinary();
			if (!data)
				continue;

			std::ofstream file(dest, std::ios::binary);
			if (file)
				file.write(reinterpret_cast<char*>(data), entry.getSize());
			else
				Log::WriteLine(Channel::Error, "Failed to write zip entry: {}", dest.string());

			free(data);
		}
		archive.close();
		return true;
	}

	bool Zip::Compress(const std::filesystem::path& sourceDir, const std::filesystem::path& zipPath)
	{
		ZipArchive archive(zipPath.string());
		if (!archive.open(ZipArchive::New))
			return false;

		for (const auto& entry : std::filesystem::recursive_directory_iterator(sourceDir))
		{
			if (!entry.is_regular_file())
				continue;

			std::string key = std::filesystem::relative(entry.path(), sourceDir).string();
			archive.addFile(key, entry.path().string());
		}
		archive.close();
		return true;
	}

	bool Zip::Add(const std::filesystem::path& zipPath, const File& file)
	{
		ZipArchive archive(zipPath.string());
		if (!archive.open(ZipArchive::Write))
			return false;

		archive.addData(file.Path.string(), file.Data.data(), file.Data.size());
		archive.close();
		return true;
	}

	bool Zip::Remove(const std::filesystem::path& zipPath, const std::string& entryPath)
	{
		ZipArchive archive(zipPath.string());
		if (!archive.open(ZipArchive::Write))
			return false;

		ZipEntry entry = archive.getEntry(entryPath);
		if (entry.isNull())
		{
			archive.close();
			return false;
		}
		archive.deleteEntry(entry);
		archive.close();
		return true;
	}

	bool Zip::Contains(const std::filesystem::path& zipPath, const std::string& entryPath)
	{
		ZipArchive archive(zipPath.string());
		if (!archive.open(ZipArchive::ReadOnly))
			return false;

		bool result = !archive.getEntry(entryPath).isNull();
		archive.close();
		return result;
	}

	File Zip::Read(const std::filesystem::path& zipPath, const std::string& entryPath)
	{
		File file;
		ZipArchive archive(zipPath.string());
		if (!archive.open(ZipArchive::ReadOnly))
			return file;

		ZipEntry entry = archive.getEntry(entryPath);
		if (entry.isNull())
		{
			archive.close();
			return file;
		}
		void* data = entry.readAsBinary();
		if (data)
		{
			file.Path = entryPath;
			file.Data =
				std::vector<uint8_t>(static_cast<uint8_t*>(data), static_cast<uint8_t*>(data) + entry.getSize());
			free(data);
		}
		archive.close();
		return file;
	}
}
