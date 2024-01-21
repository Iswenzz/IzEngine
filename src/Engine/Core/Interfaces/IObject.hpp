#pragma once
#include "Engine/Core/Serializer/ISerializable.hpp"
#include "Engine/Core/Utils/Utils.hpp"

namespace IW3SR::Engine
{
	/// <summary>
	/// Object class with serialization.
	/// </summary>
	class API IObject : public ISerializable
	{
	public:
		std::string ID = Utils::UUID();

	protected:
		/// <summary>
		/// Create a new object.
		/// </summary>
		IObject() = default;

		/// <summary>
		/// Create a new object.
		/// </summary>
		/// <param name="id">The object ID.</param>
		IObject(const std::string& id) : ID(id) { };

		/// <summary>
		/// Release the object.
		/// </summary>
		virtual ~IObject() = default;
	};
}
