#pragma once
#include "Sys/Memory.hpp"
#include "Utils/Macros.hpp"

#include <Windows.h>
#include <memory>
#include <string>

using namespace IW3SR;

namespace IW3SR
{
	/// <summary>
	/// Dynamic link library.
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
