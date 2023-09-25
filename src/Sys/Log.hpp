#pragma once
#include <format>
#include <string>
#include <iostream>

namespace IW3SR
{
	/// <summary>
	/// Log class.
	/// </summary>
	class Log
	{
	public:
		/// <summary>
		/// Initialize a new logger.
		/// </summary>
		Log() = default;
		~Log() = default;

		/// <summary>
		/// Write to the console.
		/// </summary>
		/// <param name="channel">The log channel.</param>
		/// <param name="type">The error type.</param>
		/// <param name="text">The text.</param>
		static void Write(int channel, int type, std::string text);

		/// <summary>
		/// Write to the console.
		/// </summary>
		/// <typeparam name="...Args">The format arguments.</typeparam>
		/// <param name="fmt">The format string.</param>
		/// <param name="...args">The format arguments.</param>
		template <typename... Args>
		static void Write(std::format_string<Args...> fmt, Args&&... args)
		{
			Write(0, 0, std::format(fmt, std::forward<Args>(args)...));
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
			Write(channel, 0, std::format(fmt, std::forward<Args>(args)...));
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
			Write(0, 0, std::format(fmt, std::forward<Args>(args)...) + "\n");
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
			Write(channel, 0, std::format(fmt, std::forward<Args>(args)...) + "\n");
		}
	};
}
