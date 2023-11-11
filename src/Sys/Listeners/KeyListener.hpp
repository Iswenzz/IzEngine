#pragma once
#include "Utils/Macros.hpp"

#include <Windows.h>
#include <unordered_map>
#include <nlohmann/json.hpp>

namespace IW3SR
{
	/// <summary>
	/// Key info struct.
	/// </summary>
	struct KeyInfo
	{
		int State;
		int Repeat;
	};

	/// <summary>
	/// Key listener.
	/// </summary>
	class API KeyListener
	{
	public:
		static inline std::unordered_map<int, KeyInfo> Keys;
		int Key = 0;

		/// <summary>
		/// Initialize the KeyListener.
		/// </summary>
		KeyListener() = default;

		/// <summary>
		/// Initialize the KeyLister.
		/// </summary>
		/// <param name="vk">The vk code.</param>
		KeyListener(int vk);
		~KeyListener() = default;

		/// <summary>
		/// Is key up.
		/// </summary>
		/// <returns></returns>
		bool IsUp();

		/// <summary>
		/// Is key down.
		/// </summary>
		/// <returns></returns>
		bool IsDown();

		/// <summary>
		/// Is key pressed.
		/// </summary>
		/// <returns></returns>
		bool IsPressed();

		/// <summary>
		/// Process keys.
		/// </summary>
		/// <param name="Msg">The message code.</param>
		/// <param name="wParam">Additional message-specific information.</param>
		/// <param name="lParam">Additional message-specific information.</param>
		static void Process(UINT Msg, WPARAM wParam, LPARAM lParam);

		/// <summary>
		/// Reset keys.
		/// </summary>
		static void Reset();

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(KeyListener, Key);
	};
}
