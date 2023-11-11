#include "KeyListener.hpp"

namespace IW3SR
{
	KeyListener::KeyListener(int vk)
	{
		Key = vk;
	}

	bool KeyListener::IsUp()
	{
		return Keys[Key].State == WM_KEYUP;
	}

	bool KeyListener::IsDown()
	{
		return Keys[Key].State == WM_KEYDOWN;
	}

	bool KeyListener::IsPressed()
	{
		return Keys[Key].Repeat == 1 && IsDown();
	}

	void KeyListener::Process(UINT Msg, WPARAM wParam, LPARAM lParam)
	{
		switch (Msg)
		{
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			Keys[wParam].Repeat++;
			Keys[wParam].State = WM_KEYDOWN;
			break;
		case WM_KEYUP:
		case WM_SYSKEYUP:
			Keys[wParam].Repeat = 0;
			Keys[wParam].State = WM_KEYUP;
			break;
		}
	}

	void KeyListener::Reset()
	{
		for (auto& [key, info] : Keys)
			info.State = 0;
	}
}
