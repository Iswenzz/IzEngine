#pragma once
#include <string>

namespace IW3SR
{
	/// <summary>
	/// Game module.
	/// </summary>
	class Module
	{
	public:
		std::string ID;
		std::string Name;
		bool IsEnabled = true;

		/// <summary>
		/// Initialize the module.
		/// </summary>
		virtual void Initialize() = 0;

		/// <summary>
		/// Shutdown the module.
		/// </summary>
		virtual void Shutdown() = 0;

		/// <summary>
		/// Module frame.
		/// </summary>
		virtual void Frame();
	};
}
