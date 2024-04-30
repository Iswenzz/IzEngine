#include "Windows/Base.hpp"

#include "Core/Console/Console.hpp"
#include "Core/Input/Keyboard.hpp"

namespace IzEngine
{
	void Console::Initialize()
	{
		AllocConsole();
		Handle = GetConsoleWindow();

		freopen_s(reinterpret_cast<FILE**>(stdin), "CONIN$", "r", stdin);
		freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);
		freopen_s(reinterpret_cast<FILE**>(stderr), "CONOUT$", "w", stderr);

		InputHandle = GetStdHandle(STD_INPUT_HANDLE);
		OutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		GetConsoleMode(InputHandle, reinterpret_cast<LPDWORD>(&ModeIn));
		GetConsoleMode(OutputHandle, reinterpret_cast<LPDWORD>(&ModeOut));

		ModeOut |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
		ModeIn &= ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT);

		SetConsoleMode(InputHandle, ModeIn);
		SetConsoleMode(OutputHandle, ModeOut);
	}

	void Console::Shutdown()
	{
		fclose(stdin);
		fclose(stdout);
		fclose(stderr);

		FreeConsole();
	}

	void Console::SetTitle(const std::string& title)
	{
		SetConsoleTitle(title.c_str());
	}

	void Console::AddCommand(const std::string& command)
	{
		Commands.push_back(command);
	}

	std::string Console::Input()
	{
		std::string command;

		DWORD numEventsRead;
		DWORD lpNumberOfEvents;
		INPUT_RECORD record;

		GetNumberOfConsoleInputEvents(InputHandle, &lpNumberOfEvents);
		if (!lpNumberOfEvents)
			return command;

		ReadConsoleInput(InputHandle, &record, 1, &numEventsRead);
		if (record.EventType != KEY_EVENT || !record.Event.KeyEvent.bKeyDown)
			return command;

		Key = Keyboard::Map(record.Event.KeyEvent.wVirtualKeyCode);
		const char character = record.Event.KeyEvent.uChar.AsciiChar;
		const bool print = std::isprint(character);

		switch (Key)
		{
		case Key_LeftArrow:
			InputCursorLeft();
			break;
		case Key_RightArrow:
			InputCursorRight();
			break;
		}
		Hide();

		switch (Key)
		{
		case Key_Enter:
			command = InputEnter();
			break;
		case Key_Delete:
		case Key_Backspace:
			InputBackspace();
			break;
		case Key_Tab:
			InputAutocomplete();
			break;
		case Key_UpArrow:
			InputHistoryUp();
			break;
		case Key_DownArrow:
			InputHistoryDown();
			break;
		}
		if (print && InputBuffer.size() < 256)
			InputBuffer.insert(InputBuffer.begin() + Cursor++, character);

		Show();
		PreviousKey = Key;
		return command;
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
		const auto command = Console::Input();
		if (command.empty())
			return;

		EventConsoleCommand event(command);
		Application::Get().Dispatch(event);
	}
}
