#pragma once
#include "Core/Math.hpp"

#include <string>

namespace IzEngine
{
	/// <summary>
	/// Window class.
	/// </summary>
	class Window
	{
	public:
		static inline void* Handle = nullptr;
		static inline std::string Name;
		static inline vec2 Position;
		static inline vec2 Size = { 1280, 720 };

		static inline bool Open = false;
		static inline bool IsCapture = true;

		/// <summary>
		/// Initialize window.
		/// </summary>
		/// <param name="name">The window name.</param>
		static void Initialize(const std::string& name);

		/// <summary>
		/// Window swap pointers.
		/// </summary>
		/// <param name="handle">The window handle.</param>
		static void Swap(void* handle);

		/// <summary>
		/// Shutdown window.
		/// </summary>
		static void Shutdown();

		/// <summary>
		/// Capture window.
		/// </summary>
		/// <param name="state">The capture state.</param>
		static void SetCapture(bool state);

		/// <summary>
		/// Update callback.
		/// </summary>
		/// <param name="handle">The handle.</param>
		/// <param name="msg">The message.</param>
		/// <param name="arg1">Args.</param>
		/// <param name="arg2">Args</param>
		/// <returns></returns>
		static int Update(void* handle, int msg, uintptr_t arg1, uintptr_t arg2);

		/// <summary>
		/// Window frame.
		/// </summary>
		static void Frame();

		/// <summary>
		/// Set window to borderless fullscreen.
		/// </summary>
		static void BorderlessFullscreen();

		/// <summary>
		/// Get style.
		/// </summary>
		/// <returns></returns>
		static int GetStyle();

		/// <summary>
		/// Set style.
		/// </summary>
		/// <param name="value">The style value.</param>
		/// <returns></returns>
		static void SetStyle(int value);

		/// <summary>
		/// Is style.
		/// </summary>
		/// <param name="value">The style value.</param>
		/// <returns></returns>
		static bool IsStyle(int value);

		/// <summary>
		/// Has style.
		/// </summary>
		/// <param name="value">The style value.</param>
		/// <returns></returns>
		static bool HasStyle(int value);

		/// <summary>
		/// Is cursor visible.
		/// </summary>
		/// <returns></returns>
		static bool IsCursorVisible();

	private:
		static inline bool Swapped = false;
	};
}
