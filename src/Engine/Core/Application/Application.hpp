#pragma once
#include "Engine/Base.hpp"

#include "Core/Communication/Events.hpp"

namespace IzEngine
{
	/// <summary>
	/// Application client.
	/// </summary>
	class Application
	{
		CLASS_SINGLETON(Application)
	public:
		/// <summary>
		/// Start the application.
		/// </summary>
		void Start();

		/// <summary>
		/// Shutdown the application.
		/// </summary>
		void Shutdown();

		/// <summary>
		/// Dispatch event.
		/// </summary>
		/// <param name="event">The event.s</param>
		void Dispatch(Event& event);

	private:
		/// <summary>
		/// Initialize the application.
		/// </summary>
		Application() = default;
		~Application() = default;
	};
}
