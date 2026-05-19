#pragma once
#include "Engine/Backend/ImGUI/Common.hpp"

namespace IzEngine::UC
{
	class Memory : public Frame
	{
	public:
		MemoryEditor Editor;
		uintptr_t Address = 0x400000;
		uintptr_t MaxSize = 0x1000;

		Memory();
		virtual ~Memory() = default;

		static ImU8 Read(const ImU8* data, size_t offset, void* userData);
		static void Write(ImU8* data, size_t offset, ImU8 value, void* userData);

		void OnRender();
	};
}
