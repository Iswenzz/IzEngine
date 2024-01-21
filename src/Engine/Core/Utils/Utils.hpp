#pragma once
#include <filesystem>
#include <vector>
#include <string>

namespace IW3SR::Engine
{
	/// <summary>
	/// Utility class.
	/// </summary>
	class Utils
	{
	public:
		/// <summary>
		/// Get files in a directory.
		/// </summary>
		/// <param name="directory">The directory.</param>
		/// <param name="prefix">The files prefix.</param>
		/// <param name="extension">The file extension.</param>
		/// <returns></returns>
		static std::vector<std::filesystem::path> GetFiles(const std::filesystem::path& directory,
			const std::string& prefix = "", const std::string& extension = "");

		/// <summary>
		/// Generate a unique id.
		/// </summary>
		/// <returns></returns>
		static std::string UUID();

		/// <summary>
		/// Is debug build.
		/// </summary>
		/// <returns></returns>
		static bool IsDebug();
	};
}
