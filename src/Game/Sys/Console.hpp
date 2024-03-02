#pragma once
#include "Game/Definitions.hpp"

namespace IW3SR::Game
{
	constexpr std::array<ConColor, 10> Q3Colors = {
		ConColor::Black,
		ConColor::Red,
		ConColor::Green,
		ConColor::Yellow,
		ConColor::Blue,
		ConColor::Cyan,
		ConColor::Magenta,
		ConColor::Default,
		ConColor::DarkWhite,
		ConColor::DarkYellow
	};

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
		static void Write(ConChannel channel, const char* msg, int type);

		/// <summary>
		/// Execute single command.
		/// </summary>
		/// <param name="localClientNum">The local client number.</param>
		/// <param name="controllerIndex">The controller index.</param>
		/// <param name="command">The command.</param>
		static void ExecuteSingleCommand(int localClientNum, int controllerIndex, char* command);

	private:
		/// <summary>
		/// Initialize the console.
		/// </summary>
		Console();
		~Console() = default;

		/// <summary>
		/// Converts Q3 colors to ANSI.
		/// </summary>
		/// <param name="msg">The message.</param>
		/// <returns></returns>
		static std::string Q3(const std::string& msg);
	};
}
