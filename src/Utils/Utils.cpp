#include "Utils.hpp"
#include <random>

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

	std::string Utils::UUID()
	{
		auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
		std::mt19937 gen(seed);
		const std::string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

		std::string uuid;
		for (int i = 0; i < 10; ++i) 
			uuid += chars[gen() % chars.length()];
		return uuid;
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
