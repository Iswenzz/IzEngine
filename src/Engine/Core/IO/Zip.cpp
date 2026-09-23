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

		// Relative to an absolute base: with "." or an empty destination, "..\x" normalizes to a path
		// that still starts with the base's own normal form.
		const auto base = std::filesystem::absolute(destDir).lexically_normal();
		const auto dest = (base / name).lexically_normal();
		const auto relative = dest.lexically_relative(base);

		if (relative.empty() || relative.is_absolute() || *relative.begin() == "..")
			return false;

		out = dest;
		return true;
	}

	// Streamed rather than read whole, so a large entry never needs one contiguous allocation in a
	// 32-bit process. Any entry that fails fails the extraction: a caller installing the result would
	// otherwise copy a partial set of files over a working one.
	bool Zip::Extract(const std::filesystem::path& zipPath, const std::filesystem::path& destDir)
	{
		ZipArchive archive(zipPath.string());
		if (!archive.open(ZipArchive::ReadOnly))
			return false;

		bool ok = true;
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

			std::ofstream file(dest, std::ios::binary);
			if (!file || entry.readContent(file) != LIBZIPPP_OK || !file.flush())
			{
				Log::WriteLine(Channel::Error, "Failed to extract zip entry: {}", dest.string());
				ok = false;
			}
		}
		archive.close();
		return ok;
	}

	// libzippp reads the source files inside close(), so that is where a failed write shows. A false
	// here has to be trusted: a caller may delete the sources on success.
	bool Zip::Compress(const std::filesystem::path& sourceDir, const std::filesystem::path& zipPath)
	{
		ZipArchive archive(zipPath.string());
		if (!archive.open(ZipArchive::New))
			return false;

		bool ok = true;
		std::error_code ec;
		std::filesystem::recursive_directory_iterator it(sourceDir, ec);

		for (; !ec && it != std::filesystem::recursive_directory_iterator(); it.increment(ec))
		{
			if (!it->is_regular_file())
				continue;

			// Zip entry names always use forward slashes.
			const std::string key = std::filesystem::relative(it->path(), sourceDir).generic_string();
			ok &= archive.addFile(key, it->path().string());
		}
		if (ec || !ok)
		{
			archive.discard();
			std::filesystem::remove(zipPath, ec);
			return false;
		}
		return archive.close() == LIBZIPPP_OK;
	}

	bool Zip::Add(const std::filesystem::path& zipPath, const File& file)
	{
		ZipArchive archive(zipPath.string());
		if (!archive.open(ZipArchive::Write))
			return false;

		if (!archive.addData(file.Path.string(), file.Data.data(), file.Data.size()))
		{
			archive.discard();
			return false;
		}
		return archive.close() == LIBZIPPP_OK;
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
		if (archive.deleteEntry(entry) <= 0)
		{
			archive.discard();
			return false;
		}
		return archive.close() == LIBZIPPP_OK;
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
			delete[] static_cast<char*>(data); // libzippp allocates it with new[]
		}
		archive.close();
		return file;
	}
}
