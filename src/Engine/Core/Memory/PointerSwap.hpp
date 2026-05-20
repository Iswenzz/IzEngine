#pragma once
#include "Engine/Core/Common.hpp"

namespace IzEngine
{
	struct PointerSwapEntry
	{
		void** Source;
		void* Original;
		void* Target;
	};

	class PointerSwap
	{
	public:
		void Add(const std::string& name, void** source, void* target);
		void Toggle(bool state);
		void Clear();

		bool IsActive() const;

	private:
		std::unordered_map<std::string, PointerSwapEntry> Entries;
		bool Active = false;
	};
}
