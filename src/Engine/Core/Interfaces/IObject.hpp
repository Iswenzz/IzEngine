#pragma once
#include "ISerializable.hpp"

#include "Core/Communication/Events.hpp"

namespace IW3SR::Engine
{
	/// <summary>
	/// Object class with serialization.
	/// </summary>
	class API IObject : public ISerializable
	{
	public:
		UUID ID;

		/// <summary>
		/// Event dispatch.
		/// </summary>
		/// <param name="event">The event.</param>
		virtual void OnEvent(Event& event) { }

	protected:
		/// <summary>
		/// Create a new object.
		/// </summary>
		IObject() = default;

		/// <summary>
		/// Create a new object.
		/// </summary>
		/// <param name="id">The object ID.</param>
		IObject(const UUID& id) : ID(id){};

		/// <summary>
		/// Release the object.
		/// </summary>
		virtual ~IObject() = default;
	};
}
