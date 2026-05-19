#pragma once
#include "Engine/Core/Base.hpp"

namespace IzEngine
{
	class Console
	{
	public:
		static inline void* Handle = nullptr;
		static inline void* InputHandle = nullptr;
		static inline void* OutputHandle = nullptr;
		static inline int ModeIn = 0;
		static inline int ModeOut = 0;
		static inline bool Terminated = false;

		static inline std::string InputBuffer;
		static inline std::vector<std::string> Commands;
		static inline std::vector<std::string> Autocomplete;
		static inline std::vector<std::string> History;

		static inline int Key = 0;
		static inline int PreviousKey = 0;
		static inline int Cursor = 0;
		static inline int AutocompleteIndex = 0;
		static inline bool AutocompleteBegin = false;
		static inline int HistoryIndex = 0;

		static void Initialize(const std::string& name);
		static void Shutdown();

		static void AddCommand(const std::string& command);
		static std::string Input();
		static void Show();
		static void Hide();
		static void Frame();

	private:
		static std::string InputEnter();
		static void InputBackspace();
		static void InputAutocomplete();
		static void InputHistoryUp();
		static void InputHistoryDown();
		static void InputCursorLeft();
		static void InputCursorRight();

		static void PrintForward();
		static void PrintBack();
		static void PrintBreak();

		static int SignalHandler(int signal);
	};
}
