#pragma once
#include "Game/Definitions.hpp"

namespace IW3SR::Game
{
	/// <summary>
	/// Console class.
	/// </summary>
	class Console : public IInitializable
	{
		CLASS_SINGLETON(Console)
	public:
		Engine::Console& Con;

		/// <summary>
		/// Initialize the console.
		/// </summary>
		void Initialize();

		/// <summary>
		/// Release the console.
		/// </summary>
		void Release();

		/// <summary>
		/// Write to the console.
		/// </summary>
		/// <param name="channel">The channel.</param>
		/// <param name="msg">The message.</param>
		/// <param name="type">The messsage type.</param>
		static void Write(int channel, const char* msg, int type);

	private:
		/// <summary>
		/// Initialize the console.
		/// </summary>
		Console();
		~Console() = default;

		/// <summary>
		/// Console handler.
		/// </summary>
		/// <param name="dwCtrlType">The ctrl type.</param>
		/// <returns></returns>
		static BOOL WINAPI ConsoleHandler(DWORD dwCtrlType);
	};
}
