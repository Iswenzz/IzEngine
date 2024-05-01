#pragma once
#include <array>
#include <numeric>
#include <type_traits>

namespace IzEngine
{
	/// <summary>
	/// Circular buffer.
	/// </summary>
	/// <typeparam name="T">The values type.</typeparam>
	/// <typeparam name="MaxSize">Buffer size.</typeparam>
	template <typename T, size_t MaxSize>
	struct CircularBuffer
	{
		std::array<T, MaxSize> Data{};
		size_t Offset = 0;
		size_t Size = 0;

		/// <summary>
		/// Initialize a new CircularBuffer.
		/// </summary>
		CircularBuffer() = default;
		~CircularBuffer() = default;

		/// <summary>
		/// Add an element.
		/// </summary>
		/// <param name="element">The element.</param>
		inline void Add(const T& element)
		{
			Data[Offset] = element;
			Size = Offset + 1;
			Offset = Size % MaxSize;
		}

		/// <summary>
		/// Sum numeric value.
		/// </summary>
		/// <returns></returns>
		template <typename U = T>
		requires std::is_arithmetic_v<U>
		inline U Sum()
		{
			return std::accumulate(Begin(), End(), 0);
		}

		/// <summary>
		/// Average numeric value.
		/// </summary>
		/// <returns></returns>
		template <typename U = T>
		requires std::is_arithmetic_v<T>
		inline U Average()
		{
			return Size ? Sum() / Size : 0;
		}

		/// <summary>
		/// Mode numeric value.
		/// </summary>
		/// <returns></returns>
		template <typename U = T>
		requires std::is_arithmetic_v<T>
		inline U Mode()
		{
			if (!Size)
				return 0;

			std::unordered_map<U, size_t> map;
			for (size_t i = 0; i < Size; ++i)
				++map[Data[i]];

			U value{};
			size_t maxFrequency = 0;
			for (const auto& pair : map)
			{
				if (pair.second > maxFrequency)
				{
					value = pair.first;
					maxFrequency = pair.second;
				}
			}
			return value;
		}

		/// <summary>
		/// Clear the buffer.
		/// </summary>
		constexpr inline void Clear()
		{
			Data.fill(0);
			Size = 0;
			Offset = 0;
		}

		/// <summary>
		/// Iterator begin.
		/// </summary>
		/// <returns></returns>
		constexpr inline std::array<T, MaxSize>::iterator Begin()
		{
			return Data.begin();
		}

		/// <summary>
		/// Iterator end.
		/// </summary>
		/// <returns></returns>
		constexpr inline std::array<T, MaxSize>::iterator End()
		{
			return Data.end();
		}

		/// <summary>
		/// Get the buffer data.
		/// </summary>
		/// <returns></returns>
		constexpr inline const T* Get() const
		{
			return Data.data();
		}

		/// <summary>
		/// Get the buffer max size.
		/// </summary>
		/// <returns></returns>
		constexpr inline size_t Max() const
		{
			return MaxSize;
		}
	};
}
