#pragma once
#include "Windows/Base.hpp"

namespace IW3SR::Engine
{
	/// <summary>
	/// Console class.
	/// </summary>
	class Console
	{
	public:
		static inline HWND Handle = nullptr;
		static inline HANDLE OutputHandle = nullptr;
		static inline DWORD Mode = 0;

		/// <summary>
		/// Initialize the console.
		/// </summary>
		static void Initialize();

		/// <summary>
		/// Release the console.
		/// </summary>
		static void Release();

		/// <summary>
		/// Set the console title.
		/// </summary>
		/// <param name="title">The title.</param>
		static void SetTile(const std::string& title);
	};
}
