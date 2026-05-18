#include "Console.hpp"

namespace IzEngine
{
	void Console::AddCommand(const std::string& command)
	{
		Commands.push_back(command);
	}

	std::string Console::InputEnter()
	{
		History.push_back(InputBuffer);
		HistoryIndex = History.size() - 1;

		InputBuffer.clear();
		Cursor = 0;

		return History[HistoryIndex];
	}

	void Console::InputBackspace()
	{
		if (InputBuffer.size() && Cursor)
			InputBuffer.erase(InputBuffer.begin() + --Cursor);
	}

	void Console::InputAutocomplete()
	{
		if (InputBuffer.empty())
			return;

		if (PreviousKey != Key)
			AutocompleteBegin = true;

		if (AutocompleteBegin)
		{
			Autocomplete = Commands
				| std::views::filter([&](const auto& command) { return command.starts_with(InputBuffer); })
				| std::ranges::to<std::vector<std::string>>();

			if (Autocomplete.size())
			{
				std::cout << "]" << InputBuffer << "\n";
				for (const auto& command : Autocomplete)
					std::cout << "\t" << command << "\n";
			}
			AutocompleteIndex = 0;
			AutocompleteBegin = false;
		}
		if (Autocomplete.size())
		{
			if (AutocompleteIndex >= Autocomplete.size())
				AutocompleteIndex = 0;

			InputBuffer = Autocomplete[AutocompleteIndex++];
			Cursor = InputBuffer.size();
		}
	}

	void Console::InputHistoryUp()
	{
		if (History.empty())
			return;

		InputBuffer = History[HistoryIndex];
		Cursor = InputBuffer.size();

		if (HistoryIndex)
			HistoryIndex--;
	}

	void Console::InputHistoryDown()
	{
		if (History.empty())
			return;

		if (HistoryIndex < History.size() - 1)
			HistoryIndex++;

		InputBuffer = History[HistoryIndex];
		Cursor = InputBuffer.size();
	}

	void Console::InputCursorLeft()
	{
		if (!Cursor)
			return;

		Cursor--;
		PrintBack();
	}

	void Console::InputCursorRight()
	{
		if (Cursor >= InputBuffer.size())
			return;

		Cursor++;
		PrintForward();
	}

	void Console::PrintForward()
	{
		std::cout << "\033[C";
	}

	void Console::PrintBack()
	{
		std::cout << "\033[D";
	}

	void Console::PrintBreak()
	{
		std::cout << "\b \b";
	}

	void Console::Show()
	{
		std::cout << "]" << InputBuffer;
		for (int i = 0; i < InputBuffer.size() - Cursor; ++i)
			PrintBack();
	}

	void Console::Hide()
	{
		for (int i = Cursor; i < InputBuffer.size(); ++i)
			PrintForward();
		for (int i = 0; i < InputBuffer.size() + 1; i++)
			PrintBreak();
	}

	void Console::Frame()
	{
		if (!Handle)
			return;

		const auto command = Console::Input();
		if (command.empty())
			return;

		EventConsoleCommand event(command);
		Application::Dispatch(event);
	}
}
