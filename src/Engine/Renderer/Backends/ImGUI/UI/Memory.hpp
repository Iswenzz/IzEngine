#pragma once
#include "ImGUI/Drawing/Frame.hpp"

namespace IzEngine::UC
{
	/// <summary>
	/// Memory editor frame.
	/// </summary>
	class Memory : public Frame
	{
	public:
		MemoryEditor Editor;
		uintptr_t Address = 0x400000;
		uintptr_t MaxSize = 0x1000;

		/// <summary>
		/// Initialize the memory frame.
		/// </summary>
		Memory();
		virtual ~Memory() = default;

		/// <summary>
		/// Read memory.
		/// </summary>
		/// <param name="data">The base address.</param>
		/// <param name="offset">The offset.</param>
		/// <param name="userData">The user data.</param>
		/// <returns></returns>
		static ImU8 Read(const ImU8* data, size_t offset, void* userData);

		/// <summary>
		/// Write memory.
		/// </summary>
		/// <param name="data">The base address.</param>
		/// <param name="offset">The offset.</param>
		/// <param name="value">The value.</param>
		/// <param name="userData">The user data.</param>
		static void Write(ImU8* data, size_t offset, ImU8 value, void* userData);

		/// <summary>
		/// Render frame.
		/// </summary>
		void OnRender();
	};
}
