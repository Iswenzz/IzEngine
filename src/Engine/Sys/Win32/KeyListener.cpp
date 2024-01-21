#include "KeyListener.hpp"

namespace IW3SR::Engine
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
		return Keys[Key].PrevState == 0 && IsDown();
	}

	bool KeyListener::IsUp(int vk)
	{
		return KeyListener(vk).IsUp();
	}

	bool KeyListener::IsDown(int vk)
	{
		return KeyListener(vk).IsDown();
	}

	bool KeyListener::IsPressed(int vk)
	{
		return KeyListener(vk).IsPressed();
	}

	void KeyListener::Process(UINT Msg, WPARAM wParam, LPARAM lParam)
	{
		switch (Msg)
		{
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			Keys[wParam].State = WM_KEYDOWN;
			break;
		case WM_KEYUP:
		case WM_SYSKEYUP:
			Keys[wParam].State = WM_KEYUP;
			break;
		}
	}

	void KeyListener::Reset()
	{
		for (auto& [key, info] : Keys)
		{
			info.PrevState = info.State;
			if (info.State == WM_KEYUP)
				info.State = 0;
		}
	}
}
