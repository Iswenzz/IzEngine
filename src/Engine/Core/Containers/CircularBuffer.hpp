#pragma once
#include "Engine/Core/Base.hpp"

namespace IzEngine
{
	template <typename T, size_t MaxSize>
	struct CircularBuffer
	{
		std::array<T, MaxSize> Data{};
		size_t Offset = 0;
		size_t Size = 0;

		CircularBuffer() = default;
		~CircularBuffer() = default;

		inline void Add(const T& element)
		{
			Data[Offset] = element;
			Offset = (Offset + 1) % MaxSize;
			if (Size < MaxSize)
				++Size;
		}

		template <typename U = T>
		requires std::is_arithmetic_v<U>
		inline U Sum()
		{
			return std::accumulate(Begin(), Begin() + Size, U{});
		}

		template <typename U = T>
		requires std::is_arithmetic_v<U>
		inline U Average()
		{
			return Size ? static_cast<U>(Sum<U>() / static_cast<U>(Size)) : U{};
		}

		template <typename U = T>
		requires std::is_arithmetic_v<U>
		inline U Mode()
		{
			if (!Size)
				return U{};

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

		constexpr inline void Clear()
		{
			Data.fill(T{});
			Size = 0;
			Offset = 0;
		}

		constexpr inline std::array<T, MaxSize>::iterator Begin()
		{
			return Data.begin();
		}

		constexpr inline std::array<T, MaxSize>::iterator End()
		{
			return Data.end();
		}

		constexpr inline const T* Get() const
		{
			return Data.data();
		}

		constexpr inline size_t Max() const
		{
			return MaxSize;
		}
	};
}
