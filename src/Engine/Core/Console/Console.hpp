#pragma once
#include <string>
#include <vector>

namespace IW3SR::Engine
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

		static inline std::string InputBuffer;
		static inline std::vector<std::string> Commands;
		static inline std::vector<std::string> Autocomplete;
		static inline std::vector<std::string> History;

		static inline int Cursor = 0;
		static inline int AutocompleteIndex = 0;
		static inline bool AutocompleteBegin = false;
		static inline int HistoryIndex = 0;

		/// <summary>
		/// Initialize the console.
		/// </summary>
		static void Initialize();

		/// <summary>
		/// Shutdown the console.
		/// </summary>
		static void Shutdown();

		/// <summary>
		/// Set the console title.
		/// </summary>
		/// <param name="title">The title.</param>
		static void SetTile(const std::string& title);

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
		/// Move forward.
		/// </summary>
		static void MoveForward();

		/// <summary>
		/// Move back.
		/// </summary>
		static void MoveBack();

		/// <summary>
		/// Backspace.
		/// </summary>
		static void Backspace();
	};
}
