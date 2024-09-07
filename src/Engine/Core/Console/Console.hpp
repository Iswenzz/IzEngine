#pragma once
#include <string>
#include <vector>

namespace IzEngine
{
	/// <summary>
	/// Console class.
	/// </summary>
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

		/// <summary>
		/// Initialize the console.
		/// </summary>
		/// <param name="title">The title.</param>
		static void Initialize(const std::string& name);

		/// <summary>
		/// Shutdown the console.
		/// </summary>
		static void Shutdown();

		/// <summary>
		/// Add a command.
		/// </summary>
		/// <param name="command">The command.</param>
		static void AddCommand(const std::string& command);

		/// <summary>
		/// Console input.
		/// </summary>
		/// <returns></returns>
		static std::string Input();

		/// <summary>
		/// Show input.
		/// </summary>
		static void Show();

		/// <summary>
		/// Hide input.
		/// </summary>
		static void Hide();

		/// <summary>
		/// Console frame.
		/// </summary>
		static void Frame();

	private:
		/// <summary>
		/// Input enter.
		/// </summary>
		/// <returns></returns>
		static std::string InputEnter();

		/// <summary>
		/// Input backspace.
		/// </summary>
		static void InputBackspace();

		/// <summary>
		/// Input autocomplete.
		/// </summary>
		static void InputAutocomplete();

		/// <summary>
		/// Input history up.
		/// </summary>
		static void InputHistoryUp();

		/// <summary>
		/// Input history down.
		/// </summary>
		static void InputHistoryDown();

		/// <summary>
		/// Input cursor left.
		/// </summary>
		static void InputCursorLeft();

		/// <summary>
		/// Input cursor right.
		/// </summary>
		static void InputCursorRight();

		/// <summary>
		/// Print forward.
		/// </summary>
		static void PrintForward();

		/// <summary>
		/// Print break.
		/// </summary>
		static void PrintBack();

		/// <summary>
		/// Print break.
		/// </summary>
		static void PrintBreak();

		/// <summary>
		/// Signal handler.
		/// </summary>
		/// <param name="signal">The signal.</param>
		/// <returns></returns>
		static int SignalHandler(int signal);
	};
}
