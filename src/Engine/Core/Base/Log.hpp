#pragma once
#include "Engine/Core/Base.hpp"

namespace IzEngine
{
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

	enum class LogColor
	{
		Default = 0,
		Dodger = 36,
		Gray = 37,
		Red = 91,
		Green = 92,
		Yellow = 93,
		Blue = 94,
		Magenta = 95,
		Cyan = 96,
		White = 97,
		Black = 90,
	};

	class API Log
	{
	public:
		static void Write(const std::string& msg);

		template <typename... Args>
		static void Write(std::format_string<Args...> fmt, Args&&... args)
		{
			Write(std::format(fmt, std::forward<Args>(args)...));
		}

		template <typename... Args>
		static void WriteLine(std::format_string<Args...> fmt, Args&&... args)
		{
			Write(std::format(fmt, std::forward<Args>(args)...) + '\n');
		}

		template <typename... Args>
		static void WriteLine(Channel channel, std::format_string<Args...> fmt, Args&&... args)
		{
			const auto& msg = std::format(fmt, std::forward<Args>(args)...) + '\n';
			const auto& [name, color] = Channels[channel];
			Write(std::format("{}{}{}", FormatColor(color), FormatChannel(name), msg));
		}

	private:
		struct Info
		{
			std::string name;
			LogColor color;
		};

		static inline std::unordered_map<Channel, Info> Channels = { { Channel::Log, { "Log", LogColor::Default } },
			{ Channel::Debug, { "Debug", LogColor::Gray } }, { Channel::Warning, { "Warning", LogColor::Yellow } },
			{ Channel::Error, { "Error", LogColor::Red } }, { Channel::Info, { "Info", LogColor::Cyan } },
			{ Channel::Verbose, { "Verbose", LogColor::Green } }, { Channel::System, { "System", LogColor::Dodger } },
			{ Channel::Engine, { "Engine", LogColor::Dodger } }, { Channel::Game, { "Game", LogColor::Cyan } },
			{ Channel::Audio, { "Audio", LogColor::Magenta } } };

		static inline std::string FormatChannel(const std::string& name)
		{
			return std::format("[{}] ", name);
		}

		static inline std::string FormatColor(LogColor color)
		{
			return std::format("\x1b[{}m", static_cast<int>(color));
		}
	};
}
