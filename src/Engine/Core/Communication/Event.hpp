#pragma once
#include "Engine/Core/Base.hpp"

#define EVENT_BIND(function) [this](auto&&... args) { this->function(std::forward<decltype(args)>(args)...); }
#define EVENT_BIND_VOID(function) [this]() { this->function(); }

#define EVENT_CLASS(type)                                  \
public:                                                    \
	virtual std::string_view GetEventType() const override \
	{                                                      \
		return GetStaticType();                            \
	}                                                      \
	static std::string_view GetStaticType()                \
	{                                                      \
		return type;                                       \
	}

namespace IzEngine
{
	class Event
	{
	public:
		bool PreventDefault = false;

		virtual ~Event() = default;
		virtual std::string_view GetEventType() const = 0;
	};

	template <typename C, typename E>
	concept WithEvent = requires(C callback, E& event) {
		{ callback(event) } -> std::same_as<void>;
	};

	class EventDispatcher
	{
	public:
		EventDispatcher(Event& event) : Value(event) { }

		template <typename E, typename C>
		void Dispatch(const C& callback)
		{
			if (Value.GetEventType() != E::GetStaticType())
				return;

			if constexpr (WithEvent<C, E>)
				callback(static_cast<E&>(Value));
			else
				callback();
		}

	private:
		Event& Value;
	};
}
