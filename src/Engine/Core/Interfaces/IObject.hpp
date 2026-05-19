#pragma once
#include "ISerializable.hpp"

#include "Engine/Core/Base/UUID.hpp"
#include "Engine/Core/Communication/Event.hpp"

namespace IzEngine
{
	class API IObject : public ISerializable
	{
	public:
		UUID ID;

		virtual void OnEvent(Event& event) { }

	protected:
		IObject() = default;
		IObject(const UUID& id) : ID(id) {};
		virtual ~IObject() = default;
	};
}
