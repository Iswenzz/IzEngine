#include "File.hpp"

namespace IzEngine
{
	bool File::IsValid() const
	{
		return !Data.empty();
	}

	std::string File::ReadText(const std::filesystem::path& path)
	{
		std::ifstream stream(path);

		if (!stream.is_open())
		{
			Log::WriteLine(Channel::Error, "[File] Failed to open file: {0}", path.string());
			return {};
		}
		std::stringstream buffer;
		buffer << stream.rdbuf();
		return buffer.str();
	}

	File File::Read(const std::filesystem::path& path)
	{
		File file;
		file.Path = path;

		std::ifstream stream(path, std::ios::binary | std::ios::ate);

		if (!stream.is_open())
		{
			Log::WriteLine(Channel::Error, "[File] Failed to open file: {0}", path.string());
			return file;
		}
		std::streamsize size = stream.tellg();
		stream.seekg(0, std::ios::beg);

		file.Data.resize(size);
		stream.read(reinterpret_cast<char*>(file.Data.data()), size);

		return file;
	}
}
