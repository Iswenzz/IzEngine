#pragma once
#include "Engine/Core/Common.hpp"

namespace IzEngine
{
	class Window
	{
	public:
		static inline void* Handle = nullptr;
		static inline std::string Name;
		static inline vec2 Position;
		static inline vec2 Size = { 1280, 720 };

		static inline bool Open = false;
		static inline bool IsCapture = true;

		static void Initialize(const std::string& name);
		static void Swap(void* handle);
		static void Shutdown();

		static int Update(void* handle, int msg, uintptr_t arg1, uintptr_t arg2);
		static void Frame();
		static void BorderlessFullscreen();

		static int GetStyle();
		static void SetStyle(int value);
		static void SetCapture(bool state);
		static bool IsStyle(int value);
		static bool HasStyle(int value);
		static bool IsCursorVisible();

	private:
		static inline bool Swapped = false;
	};
}
