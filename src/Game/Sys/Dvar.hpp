#pragma once
#include "Game/Definitions.hpp"

namespace IW3SR::Game
{
	/// <summary>
	/// Game dvars.
	/// </summary>
	class Dvar
	{
	public:
		/// <summary>
		/// Get the dvar.
		/// </summary>
		/// <typeparam name="T">The value type.</typeparam>
		/// <param name="name">The dvar name.</param>
		template <typename T>
		static T Get(const std::string& name)
		{
			dvar_s* dvar = Dvar_FindVar(name.c_str());
			if (!dvar)
			{
				throw std::runtime_error("dvar not found");
				return 0;
			}
			return *reinterpret_cast<T*>(&dvar->current.value);
		}

		/// <summary>
		/// Set the dvar.
		/// </summary>
		/// <typeparam name="T">The value type.</typeparam>
		/// <param name="name">The dvar name.</param>
		/// <param name="value">The value.</param>
		template <typename T>
		static void Set(const std::string& name, T value)
		{
			dvar_s* dvar = Dvar_FindVar(name.c_str());
			if (!dvar)
			{
				throw std::runtime_error("dvar not found");
				return;
			}
			Memory::Assign<T>(dvar->current.value, value);
			Memory::Assign<T>(dvar->latched.value, value);
		}
	};
}
