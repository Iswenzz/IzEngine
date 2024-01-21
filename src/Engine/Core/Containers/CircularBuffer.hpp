#pragma once
#include <array>

namespace IW3SR::Engine
{
	/// <summary>
	/// Circular buffer.
	/// </summary>
	/// <typeparam name="T">The values type.</typeparam>
	/// <typeparam name="MaxSize">Buffer size.</typeparam>
	template <typename T, size_t MaxSize>
	struct CircularBuffer
	{
		size_t Offset = 0;
		std::array<T, MaxSize> Data{ };

		/// <summary>
		/// Initialize a new CircularBuffer.
		/// </summary>
		CircularBuffer() = default;
		virtual ~CircularBuffer() = default;

		/// <summary>
		/// Add an element.
		/// </summary>
		/// <param name="element">The element.</param>
		void Add(const T& element)
		{
			Data[Offset] = element;
			Offset = (Offset + 1) % MaxSize;
		}

		/// <summary>
		/// Clear the buffer.
		/// </summary>
		void Clear()
		{
			Data.fill(0);
			Offset = 0;
		}

		/// <summary>
		/// Get the buffer data.
		/// </summary>
		/// <returns></returns>
		constexpr const T* Get() const
		{
			return Data.data();
		}

		/// <summary>
		/// Get the buffer size.
		/// </summary>
		/// <returns></returns>
		constexpr size_t Size() const
		{
			return MaxSize;
		}
	};
}
