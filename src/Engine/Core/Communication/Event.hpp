#pragma once
#include <concepts>

#define EVENT_BIND(function) [this](auto&&... args) \
	{ this->function(std::forward<decltype(args)>(args)...); }

#define EVENT_BIND_VOID(function) [this]() \
	{ this->function(); }

#define EVENT_CLASS(type) \
	public: \
	virtual const char* GetEventType() const override { return GetStaticType(); } \
	static const char* GetStaticType() { return type; }

namespace IW3SR::Engine
{
	/// <summary>
	/// Event class.
	/// </summary>
	class Event
	{
	public:
		/// <summary>
		/// Release event.
		/// </summary>
		virtual ~Event() = default;

		/// <summary>
		/// Get the event type.
		/// </summary>
		/// <returns></returns>
		virtual const char* GetEventType() const = 0;
	};

	/// <summary>
	/// Callable with event.
	/// </summary>
	template <typename C, typename E>
	concept WithEvent = requires(C callback, E& event) 
	{
		{ callback(event) } -> std::same_as<void>;
	};

	/// <summary>
	/// Event dispatcher.
	/// </summary>
	class EventDispatcher
	{
	public:
		/// <summary>
		/// Create an event dispatcher.
		/// </summary>
		/// <param name="event">The event.</param>
		EventDispatcher(Event& event) : _Event(event) { }

		/// <summary>
		/// Dispatch the event.
		/// </summary>
		/// <typeparam name="E">The event.</typeparam>
		/// <typeparam name="C">The callback.</typeparam>
		/// <param name="callback">The callback.</param>
		/// <returns></returns>
		template <typename E, typename C>
		void Dispatch(const C& callback)
		{
			if (_Event.GetEventType() != E::GetStaticType())
				return;

			if constexpr (WithEvent<C, E>)
				callback(static_cast<E&>(_Event));
			else
				callback();
		}

	private:
		Event& _Event;
	};
}
