#pragma once
#include "Sys/Function.hpp"
#include "Utils/Macros.hpp"

#include <Windows.h>
#include <memory>
#include <string>

namespace IW3SR
{
	enum class DLLMode
	{
		DEFAULT,
		PERSISTED
	};

	/// <summary>
	/// Dynamic link library.
	/// </summary>
	class DLL
	{
	public:
		std::string ID;
		std::string Name;
		std::string FilePath;

		HINSTANCE Instance;
		DLLMode Mode;
		Function<void(DLL* entry)> Initialize;
		Function<void()> GUI;
		Function<void()> Shutdown;

		/// <summary>
		/// Initialize the DLL.
		/// </summary>
		DLL() = default;
		
		/// <summary>
		/// Initialize the DLL.
		/// </summary>
		/// <param name="filePath">The DLL file path.</param>
		DLL(std::string filePath);

		/// <summary>
		/// Shutdown the DLL.
		/// </summary>
		~DLL();
	};
}
