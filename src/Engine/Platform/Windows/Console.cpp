#include "Base.hpp"

#include "Engine/Core/Console/Console.hpp"
#include "Engine/Core/Input/Input.hpp"

namespace IzEngine
{
	void Console::Initialize(const std::string& name)
	{
		AllocConsole();
		auto handler = [](DWORD signal) { return static_cast<BOOL>(SignalHandler(static_cast<int>(signal))); };
		SetConsoleCtrlHandler(handler, TRUE);
		Handle = GetConsoleWindow();
		SetConsoleTitle(name.c_str());

		FILE* stream = nullptr;
		freopen_s(&stream, "CONIN$", "r", stdin);
		freopen_s(&stream, "CONOUT$", "w", stdout);
		freopen_s(&stream, "CONOUT$", "w", stderr);

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

		Key = Input::MapKey(record.Event.KeyEvent.wVirtualKeyCode);
		const char character = record.Event.KeyEvent.uChar.AsciiChar;
		const bool print = std::isprint(static_cast<unsigned char>(character));

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

	int Console::SignalHandler(int signal)
	{
		if (signal == CTRL_CLOSE_EVENT)
			Terminated = true;
		return TRUE;
	}
}
