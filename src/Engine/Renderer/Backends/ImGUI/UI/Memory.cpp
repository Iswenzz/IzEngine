#include "Memory.hpp"
#include "Core/Memory/Memory.hpp"

namespace IzEngine::UC
{
	Memory::Memory() : Frame("Memory")
	{
		SetRect(20, 20, 240, 350);
		Editor.OptMidColsCount = 4;
		Editor.OptAddrDigitsCount = sizeof(uintptr_t) * 2;
		Editor.OptShowDataPreview = true;
		Editor.ReadFn = Read;
		Editor.WriteFn = Write;
	}

	ImU8 Memory::Read(const ImU8* data, size_t offset, void* userData)
	{
		ImU8 byte = 0;
		IzEngine::Memory::Read(reinterpret_cast<uintptr_t>(data + offset), &byte, 1);
		return byte;
	}

	void Memory::Write(ImU8* data, size_t offset, ImU8 value, void* userData)
	{
		IzEngine::Memory::Write(reinterpret_cast<uintptr_t>(data + offset), reinterpret_cast<char*>(&value));
	}

	void Memory::OnRender()
	{
		const int speed = Editor.Cols * 5;
		const int scroll = ImGui::GetIO().MouseWheel;
		const int direction = scroll ? scroll > 0 ? 1 : -1 : 0;
		const int step = 1;
		const int stepFast = speed * speed;
		const char* format = sizeof(uintptr_t) == 8 ? "%016X" : "%08X";

		Address += uintptr_t(-direction * speed);

		Begin();
		ImGui::InputScalar("Address", ImGuiDataType_U64, &Address, &step, &stepFast, format);
		Editor.DrawContents(reinterpret_cast<void*>(Address), MaxSize, Address);
		End();
	}
}
