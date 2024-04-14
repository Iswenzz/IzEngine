#pragma once
#include "Macros.hpp"

#include <format>
#include <iostream>
#include <string>
#include <unordered_map>

namespace IW3SR::Engine
{
	/// <summary>
	/// Channels.
	/// </summary>
	enum class Channel
	{
		Log,
		Debug,
		Warning,
		Error,
		Info,
		Verbose,
		System,
		Engine,
		Game,
		Audio
	};

	/// <summary>
	/// Log ANSI colors.
	/// </summary>
	enum class LogColor
	{
		Default = 0,
		Red = 91,
		Green = 92,
		Yellow = 93,
		Blue = 94,
		Magenta = 95,
		Cyan = 96,
		White = 97,
		Black = 30,
		DarkRed = 31,
		DarkGreen = 32,
		DarkYellow = 33,
		DarkBlue = 34,
		DarkMagenta = 35,
		DarkCyan = 36,
		DarkWhite = 37,
	};

	/// <summary>
	/// Log class.
	/// </summary>
	class API Log
	{
	public:
		/// <summary>
		/// Write to the console.
		/// </summary>
		/// <param name="msg">The string.</param>
		static void Write(const std::string& msg);

		/// <summary>
		/// Write to the console.
		/// </summary>
		/// <typeparam name="...Args">The format arguments.</typeparam>
		/// <param name="fmt">The format string.</param>
		/// <param name="...args">The format arguments.</param>
		template <typename... Args>
		static void Write(std::format_string<Args...> fmt, Args&&... args)
		{
			Write(std::format(fmt, std::forward<Args>(args)...));
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
			Write(std::format(fmt, std::forward<Args>(args)...) + '\n');
		}

		/// <summary>
		/// Write line to the console.
		/// </summary>
		/// <typeparam name="...Args">The format arguments.</typeparam>
		/// <param name="channel">The log channel.</param>
		/// <param name="fmt">The format string.</param>
		/// <param name="...args">The format arguments.</param>
		template <typename... Args>
		static void WriteLine(Channel channel, std::format_string<Args...> fmt, Args&&... args)
		{
			const auto& msg = std::format(fmt, std::forward<Args>(args)...) + '\n';
			const auto& [name, color] = Channels[channel];
			Write(std::format("{}{}{}", FormatColor(color), FormatChannel(name), msg));
		}

	private:
		/// <summary>
		/// Channel info.
		/// </summary>
		struct Info
		{
			std::string name;
			LogColor color;
		};

		// clang-format off
		/// <summary>
		/// Channels.
		/// </summary>
		static inline std::unordered_map<Channel, Info> Channels = {
			{ Channel::Log, { "Log", LogColor::Default } },
			{ Channel::Debug, { "Debug", LogColor::DarkWhite } },
			{ Channel::Warning, { "Warning", LogColor::Yellow } },
			{ Channel::Error, { "Error", LogColor::Red } },
			{ Channel::Info, { "Info", LogColor::Cyan } },
			{ Channel::Verbose, { "Verbose", LogColor::Green } },
			{ Channel::System, { "System", LogColor::Magenta } },
			{ Channel::Engine, { "Engine", LogColor::Magenta } },
			{ Channel::Game, { "Game", LogColor::Cyan } },
			{ Channel::Audio, { "Audio", LogColor::Magenta } }
		};
		// clang-format on

		/// <summary>
		/// Format channel name.
		/// </summary>
		/// <param name="channel">The channel name.</param>
		/// <returns></returns>
		static inline std::string FormatChannel(const std::string& name)
		{
			return std::format("[{}] ", name);
		}

		/// <summary>
		/// Format color.
		/// </summary>
		/// <param name="color">The color.</param>
		static inline std::string FormatColor(LogColor color)
		{
			return std::format("\x1b[{}m", static_cast<int>(color));
		}
	};
}
