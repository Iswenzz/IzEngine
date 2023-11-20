#include "Utils.hpp"

namespace IW3SR
{
	std::vector<std::filesystem::path> Utils::GetFiles(const std::filesystem::path& directory, 
		const std::string& prefix, const std::string& extension)
	{
		std::vector<std::filesystem::path> files;
		for (const auto& entry : std::filesystem::directory_iterator(directory)) 
		{
			const auto filename = entry.path().filename();
			if (entry.is_regular_file() && filename.string().find(prefix) != std::string::npos)
			{
				if (extension.empty() || filename.extension() == extension)
					files.push_back(entry.path());
			}
		}
		std::sort(files.begin(), files.end());
		return files;
	}

	bool Utils::IsDebug()
	{
		#ifdef _DEBUG
			return true;
		#else
			return false;
		#endif
	}
}
