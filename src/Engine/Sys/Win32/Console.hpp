#pragma once
#include "Engine/Sys/Win32.hpp"

namespace IW3SR::Engine
{
	/// <summary>
	/// Console class.
	/// </summary>
	class Console
	{
		CLASS_SINGLETON(Console)
	public:
		HWND Handle = nullptr;
		HANDLE OutputHandle = nullptr;
		DWORD Mode = 0;

		/// <summary>
		/// Initialize the console.
		/// </summary>
		void Initialize();

		/// <summary>
		/// Release the console.
		/// </summary>
		void Release();

		/// <summary>
		/// Set the console title.
		/// </summary>
		/// <param name="title">The title.</param>
		void SetTile(const std::string& title);

	private:
		/// <summary>
		/// Initialize the console.
		/// </summary>
		Console() = default;
		~Console() = default;
	};
}
