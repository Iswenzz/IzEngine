#include "Memory.hpp"
#include "Core/Memory/Memory.hpp"

namespace IzEngine::UC
{
	Memory::Memory() : Window("Memory")
	{
		SetRect(20, 20, 240, 350);
		Editor.OptMidColsCount = 4;
		Editor.OptAddrDigitsCount = sizeof(uintptr_t) * 2;
		Editor.OptShowDataPreview = true;
		Editor.ReadFn = Read;
		Editor.WriteFn = Write;
	}

	ImU8 Memory::Read(const ImU8* data, size_t offset)
	{
		ImU8 byte = 0;
		IzEngine::Memory::Read(reinterpret_cast<uintptr_t>(data + offset), &byte, 1);
		return byte;
	}

	void Memory::Write(ImU8* data, size_t offset, ImU8 value)
	{
		IzEngine::Memory::Write(reinterpret_cast<uintptr_t>(data + offset), reinterpret_cast<char*>(&value));
	}

	void Memory::Render()
	{
		if (!Open)
			return;

		int speed = Editor.Cols * 5;
		int scroll = ImGui::GetIO().MouseWheel;
		int direction = scroll ? scroll > 0 ? 1 : -1 : 0;
		Address += -direction * speed;

		Begin();
		ImGui::InputInt("Address", &Address, 1, speed * speed, ImGuiInputTextFlags_CharsHexadecimal);
		Editor.DrawContents(reinterpret_cast<void*>(Address), MaxSize, Address);
		End();
	}
}
