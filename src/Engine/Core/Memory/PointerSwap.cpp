#include "PointerSwap.hpp"

namespace IzEngine
{
	void PointerSwap::Add(const std::string& name, void** source, void* target)
	{
		IZ_ASSERT(source, "Source pointer is null.");
		IZ_ASSERT(target, "Target pointer is null.");

		PointerSwapEntry entry;
		entry.Source = source;
		entry.Original = *source;
		entry.Target = target;
		Entries[name] = entry;

		*source = target;
		Active = true;
	}

	void PointerSwap::Toggle(bool state)
	{
		if (Active == state)
			return;

		for (auto& [name, entry] : Entries)
			*entry.Source = state ? entry.Target : entry.Original;

		Active = state;
	}

	void PointerSwap::Clear()
	{
		Toggle(false);
		Entries.clear();
	}

	bool PointerSwap::IsActive() const
	{
		return Active;
	}
}
