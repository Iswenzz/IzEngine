#pragma once
#include <format>
#include <string>
#include <iostream>

namespace IW3SR
{
	/// <summary>
	/// Log class.
	/// </summary>
	class API Log
	{
	public:
		/// <summary>
		/// Write to the console.
		/// </summary>
		/// <param name="channel">The channel.</param>
		/// <param name="msg">The string.</param>
		static void Write(int channel, const std::string& msg)
		{
			std::cout << msg;
		}

		/// <summary>
		/// Write to the console.
		/// </summary>
		/// <typeparam name="...Args">The format arguments.</typeparam>
		/// <param name="fmt">The format string.</param>
		/// <param name="...args">The format arguments.</param>
		template <typename... Args>
		static void Write(std::format_string<Args...> fmt, Args&&... args)
		{
			Write(0, std::format(fmt, std::forward<Args>(args)...));
		}

		/// <summary>
		/// Write to the console.
		/// </summary>
		/// <typeparam name="...Args">The format arguments.</typeparam>
		/// <param name="channel">The log channel.</param>
		/// <param name="fmt">The format string.</param>
		/// <param name="...args">The format arguments.</param>
		template <typename... Args>
		static void Write(int channel, std::format_string<Args...> fmt, Args&&... args)
		{
			Write(channel, std::format(fmt, std::forward<Args>(args)...));
		}

		/// <summary>
		/// Write line to the console.
		/// </summary>
		/// <typeparam name="...Args">The format arguments.</typeparam>
		/// <param name="fmt">The format string.</param>
		/// <param name="...args">The format arguments.</param>
		template <typename... Args>
		static void WriteLine(std::format_string<Args...> fmt, Args&&... args)
		{
			Write(0, std::format(fmt, std::forward<Args>(args)...) + "\n");
		}

		/// <summary>
		/// Write line to the console.
		/// </summary>
		/// <typeparam name="...Args">The format arguments.</typeparam>
		/// <param name="channel">The log channel.</param>
		/// <param name="fmt">The format string.</param>
		/// <param name="...args">The format arguments.</param>
		template <typename... Args>
		static void WriteLine(int channel, std::format_string<Args...> fmt, Args&&... args)
		{
			Write(channel, std::format(fmt, std::forward<Args>(args)...) + "\n");
		}
	};
}
