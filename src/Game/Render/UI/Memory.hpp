#pragma once
#include "Game/Render/GUI.hpp"

namespace IW3SR::UI
{
	/// <summary>
	/// Memory editor.
	/// </summary>
	class Memory : public Window
	{
	public:
		MemoryEditor Editor;
		int Address = 0x400000;
		int MaxSize = 0x1000;

		/// <summary>
		/// Initialize the Memory.
		/// </summary>
		Memory();
		virtual ~Memory() = default;

		/// <summary>
		/// Read memory.
		/// </summary>
		/// <param name="data">The base address.</param>
		/// <param name="offset">The offset.</param>
		/// <returns></returns>
		static ImU8 Read(const ImU8* data, size_t offset);

		/// <summary>
		/// Write memory.
		/// </summary>
		/// <param name="data">The base address.</param>
		/// <param name="offset">The offset.</param>
		/// <param name="value">The value.</param>
		static void Write(ImU8* data, size_t offset, ImU8 value);

		/// <summary>
		/// Render frame.
		/// </summary>
		void Frame();

		NLOHMANN_SERIALIZE_DERIVED(Memory, Window, Address)
	};
}
