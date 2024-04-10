#pragma once
#include "Game/Base.hpp"

namespace IW3SR::Game
{
	/// <summary>
	/// Game dvars.
	/// </summary>
	class API Dvar
	{
	public:
		/// <summary>
		/// Register int dvar.
		/// </summary>
		/// <param name="name">The dvar name.</param>
		/// <param name="flags">The dvar flags.</param>
		/// <param name="description">The description.</param>
		/// <param name="value">The default value.</param>
		/// <param name="min">The min value.</param>
		/// <param name="max">The max value.</param>
		/// <returns></returns>
		static inline dvar_s* RegisterInt(const char* name, DvarFlags flags, const char* description, int value,
			int min, int max)
		{
			return Dvar_RegisterVariantInt(name, DvarType::INTEGER, flags, description, value, 0, 0, 0, min, max);
		}

		/// <summary>
		/// Register float dvar.
		/// </summary>
		/// <param name="name">The dvar name.</param>
		/// <param name="flags">The dvar flags.</param>
		/// <param name="description">The description.</param>
		/// <param name="value">The default value.</param>
		/// <param name="min">The min value.</param>
		/// <param name="max">The max value.</param>
		/// <returns></returns>
		static inline dvar_s* RegisterFloat(const char* name, DvarFlags flags, const char* description, float value,
			float min, float max)
		{
			return Dvar_RegisterVariantFloat(name, DvarType::VALUE, flags, description, value, 0, 0, 0, min, max);
		}

		/// <summary>
		/// Register bool dvar.
		/// </summary>
		/// <param name="name">The dvar name.</param>
		/// <param name="flags">The dvar flags.</param>
		/// <param name="description">The description.</param>
		/// <param name="value">The default value.</param>
		/// <returns></returns>
		static inline dvar_s* RegisterBool(const char* name, DvarFlags flags, const char* description, bool value)
		{
			return Dvar_RegisterVariantBool(name, DvarType::BOOLEAN, flags, description, value, 0, 0, 0, 0, 0);
		}

		/// <summary>
		/// Register string dvar.
		/// </summary>
		/// <param name="name">The dvar name.</param>
		/// <param name="flags">The dvar flags.</param>
		/// <param name="description">The description.</param>
		/// <param name="value">The default value.</param>
		/// <returns></returns>
		static inline dvar_s* RegisterString(const char* name, DvarFlags flags, const char* description,
			const char* value)
		{
			return Dvar_RegisterVariantString(name, DvarType::STRING, flags, description, value, 0, 0, 0, 0, 0);
		}

		/// <summary>
		/// Register enum dvar.
		/// </summary>
		/// <param name="name">The dvar name.</param>
		/// <param name="flags">The dvar flags.</param>
		/// <param name="description">The description.</param>
		/// <param name="value">The default value.</param>
		/// <param name="list">The enum list.</param>
		/// <returns></returns>
		static inline dvar_s* RegisterEnum(const char* name, DvarFlags flags, const char* description, int value,
			const std::vector<const char*>& list)
		{
			return Dvar_RegisterVariantEnum(name, DvarType::ENUMERATION, flags, description, value, 0, 0, 0,
				static_cast<int>(list.size()), const_cast<const char**>(list.data()));
		}

		/// <summary>
		/// Register vec2 dvar.
		/// </summary>
		/// <param name="name">The dvar name.</param>
		/// <param name="flags">The dvar flags.</param>
		/// <param name="description">The description.</param>
		/// <param name="x">The default x value.</param>
		/// <param name="y">The default y value.</param>
		/// <param name="min">The min value.</param>
		/// <param name="max">The max value.</param>
		/// <returns></returns>
		static inline dvar_s* RegisterVec2(const char* name, DvarFlags flags, const char* description, float x, float y,
			float min, float max)
		{
			return Dvar_RegisterVariantVec2(name, DvarType::VEC2, flags, description, x, y, 0, 0, min, max);
		}

		/// <summary>
		/// Register vec3 dvar.
		/// </summary>
		/// <param name="name">The dvar name.</param>
		/// <param name="flags">The dvar flags.</param>
		/// <param name="description">The description.</param>
		/// <param name="x">The default x value.</param>
		/// <param name="y">The default y value.</param>
		/// <param name="z">The default z value.</param>
		/// <param name="min">The min value.</param>
		/// <param name="max">The max value.</param>
		/// <returns></returns>
		static inline dvar_s* RegisterVec3(const char* name, DvarFlags flags, const char* description, float x, float y,
			float z, float min, float max)
		{
			return Dvar_RegisterVariantVec3(name, DvarType::VEC3, flags, description, x, y, z, 0, min, max);
		}

		/// <summary>
		/// Register vec4 dvar.
		/// </summary>
		/// <param name="name">The dvar name.</param>
		/// <param name="flags">The dvar flags.</param>
		/// <param name="description">The description.</param>
		/// <param name="x">The default x value.</param>
		/// <param name="y">The default y value.</param>
		/// <param name="z">The default z value.</param>
		/// <param name="w">The default w value.</param>
		/// <param name="min">The min value.</param>
		/// <param name="max">The max value.</param>
		/// <returns></returns>
		static inline dvar_s* RegisterVec4(const char* name, DvarFlags flags, const char* description, float x, float y,
			float z, float w, float min, float max)
		{
			return Dvar_RegisterVariantVec4(name, DvarType::VEC4, flags, description, x, y, z, w, min, max);
		}

		/// <summary>
		/// Register color dvar.
		/// </summary>
		/// <param name="name">The dvar name.</param>
		/// <param name="flags">The dvar flags.</param>
		/// <param name="description">The description.</param>
		/// <param name="r">The default r value.</param>
		/// <param name="g">The default g value.</param>
		/// <param name="b">The default b value.</param>
		/// <param name="a">The default a value.</param>
		/// <returns></returns>
		static inline dvar_s* RegisterColor(const char* name, DvarFlags flags, const char* description, float r,
			float g, float b, float a)
		{
			return Dvar_RegisterVariantColor(name, DvarType::COLOR, flags, description, r, g, b, a, 0, 0);
		}

		/// <summary>
		/// Find dvar.
		/// </summary>
		/// <param name="name">The dvar name.</param>
		static inline dvar_s* Find(const std::string& name)
		{
			return Dvar_FindVar(name.c_str());
		}

		/// <summary>
		/// Get dvar value.
		/// </summary>
		/// <typeparam name="T">The value type.</typeparam>
		/// <param name="name">The dvar name.</param>
		template <typename T>
		static T Get(const std::string& name)
		{
			const auto dvar = Find(name);
			if (!dvar)
				throw std::runtime_error("Dvar not found");
			return *reinterpret_cast<T*>(&dvar->current);
		}

		/// <summary>
		/// Set dvar value.
		/// </summary>
		/// <typeparam name="T">The value type.</typeparam>
		/// <param name="name">The dvar name.</param>
		/// <param name="value">The value.</param>
		template <typename T>
		static void Set(const std::string& name, T value)
		{
			const auto dvar = Find(name);
			if (!dvar)
				throw std::runtime_error("Dvar not found");

			*reinterpret_cast<T*>(&dvar->current) = value;
			*reinterpret_cast<T*>(&dvar->latched) = value;
		}

		/// <summary>
		/// Set dvar latched value.
		/// </summary>
		/// <typeparam name="T">The value type.</typeparam>
		/// <param name="name">The dvar name.</param>
		/// <param name="value">The value.</param>
		template <typename T>
		static void SetLatched(const std::string& name, T value)
		{
			const auto dvar = Find(name);
			if (!dvar)
				throw std::runtime_error("Dvar not found");

			*reinterpret_cast<T*>(&dvar->latched) = value;
		}
	};
}
