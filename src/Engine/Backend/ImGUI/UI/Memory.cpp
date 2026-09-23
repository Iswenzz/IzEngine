#include "Memory.hpp"

#include "Engine/Core/Memory/Memory.hpp"

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
		// One byte: the string overload would stop at a zero and run on into whatever follows.
		IzEngine::Memory::Set<ImU8>(reinterpret_cast<uintptr_t>(data + offset), value);
	}

	void Memory::OnRender()
	{
		const int speed = Editor.Cols * 5;
		const float scroll = ImGui::GetIO().MouseWheel;
		const int direction = scroll ? scroll > 0 ? 1 : -1 : 0;

		// Sized to the pointer: a 64-bit scalar over a 32-bit address wrote into the member after it.
		const uintptr_t step = 1;
		const uintptr_t stepFast = speed * speed;
		const ImGuiDataType type = sizeof(uintptr_t) == 8 ? ImGuiDataType_U64 : ImGuiDataType_U32;
		const char* format = sizeof(uintptr_t) == 8 ? "%016llX" : "%08X";

		Begin();
		if (ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows))
			Address += uintptr_t(-direction * speed);

		ImGui::InputScalar("Address", type, &Address, &step, &stepFast, format);
		Editor.DrawContents(reinterpret_cast<void*>(Address), MaxSize, Address);
		End();
	}
}
