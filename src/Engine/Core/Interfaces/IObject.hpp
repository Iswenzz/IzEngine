#pragma once
#include "Engine/Core/Serializer/ISerializable.hpp"

namespace IW3SR
{
	/// <summary>
	/// Object class with serialization.
	/// </summary>
	class API IObject : public ISerializable
	{
	protected:
		/// <summary>
		/// Create a new object.
		/// </summary>
		IObject() = default;

		/// <summary>
		/// Release the object.
		/// </summary>
		virtual ~IObject() = default;
	};
}
