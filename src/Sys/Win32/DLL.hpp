#pragma once
#include "Sys/Win32.hpp"

namespace IW3SR
{
	/// <summary>
	/// Dynamic Link Library.
	/// </summary>
	class API DLL
	{
	public:
		std::string ID;
		std::string Name;
		std::string FilePath;

		HINSTANCE Instance = nullptr;
		Function<void(DLL* entry)> CallbackInitialize;
		Function<void()> CallbackGUI;
		Function<void()> CallbackShutdown;

		/// <summary>
		/// Create a new DLL.
		/// </summary>
		DLL() = default;

		/// <summary>
		/// Create a new DLL.
		/// </summary>
		/// <param name="filePath">The DLL file path.</param>
		DLL(std::string filePath);

		/// <summary>
		/// Release the DLL.
		/// </summary>
		~DLL();

		/// <summary>
		/// Set ImGUI renderer.
		/// </summary>
		void SetRenderer();

		/// <summary>
		/// Set DLL infos.
		/// </summary>
		/// <param name="id">The ID.</param>
		/// <param name="name">The name.</param>
		void SetInfos(const std::string& id, const std::string& name);
	};
}
