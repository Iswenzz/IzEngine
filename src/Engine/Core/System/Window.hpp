#pragma once
#include "Core/Math.hpp"

#include <string>

namespace IzEngine
{
	/// <summary>
	/// Window class.
	/// </summary>
	class OSWindow
	{
	public:
		static inline void* Handle = nullptr;
		static inline std::string Name;
		static inline vec2 Position;
		static inline vec2 Size;

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
		/// <param name="wParam">The w param.</param>
		/// <param name="lParam">The l param.</param>
		/// <returns></returns>
		static int Update(void* handle, int msg, uintptr_t wParam, uintptr_t lParam);

		/// <summary>
		/// Window frame.
		/// </summary>
		static void Frame();
	};
}
