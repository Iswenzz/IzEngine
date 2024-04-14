#include "Windows/Base.hpp"

#include "Core/Console/Console.hpp"
#include "Core/Input/Keyboard.hpp"

namespace IW3SR::Engine
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

	void Console::SetTile(const std::string& title)
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

		static int previousKey = 0;
		const int key = Keyboard::Map(record.Event.KeyEvent.wVirtualKeyCode);
		const char character = record.Event.KeyEvent.uChar.AsciiChar;
		const bool print = std::isprint(character);

		Hide();
		if (key == Key_Enter)
		{
			command = std::string(InputBuffer.begin(), InputBuffer.end());
			InputBuffer.clear();
			History.push_back(command);
			HistoryIndex = History.size() - 1;
			Cursor = 0;
		}
		else if (key == Key_Backspace || key == Key_Delete)
		{
			if (InputBuffer.size() && Cursor)
				InputBuffer.erase(InputBuffer.begin() + --Cursor);
		}
		else if (key == Key_Tab)
		{
			if (previousKey != key)
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
		else if (key == Key_UpArrow && !History.empty())
		{
			if (HistoryIndex)
				HistoryIndex--;

			InputBuffer = History[HistoryIndex];
			Cursor = InputBuffer.size();
		}
		else if (key == Key_DownArrow && !History.empty())
		{
			if (HistoryIndex < History.size() - 1)
				HistoryIndex++;

			InputBuffer = History[HistoryIndex];
			Cursor = InputBuffer.size();
		}
		Show();

		if (key == Key_LeftArrow && Cursor)
		{
			Cursor--;
			MoveBack();
		}
		else if (key == Key_RightArrow && Cursor < InputBuffer.size())
		{
			Cursor++;
			MoveForward();
		}
		previousKey = key;

		if (!print || InputBuffer.size() >= 256)
			return command;

		Hide();
		InputBuffer.insert(InputBuffer.begin() + Cursor++, character);
		Show();
		return command;
	}

	void Console::MoveForward()
	{
		std::cout << "\033[C";
	}

	void Console::MoveBack()
	{
		std::cout << "\033[D";
	}

	void Console::Backspace()
	{
		std::cout << "\b \b";
	}

	void Console::Show()
	{
		std::cout << "]" << InputBuffer;
		for (int i = 0; i < InputBuffer.size() - Cursor; ++i)
			MoveBack();
	}

	void Console::Hide()
	{
		for (int i = Cursor; i < InputBuffer.size(); ++i)
			MoveForward();
		for (int i = 0; i < InputBuffer.size() + 1; i++)
			Backspace();
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
