#pragma once
#include <string>

#define MODULE_INFO(id, name) public: std::string ID = id; std::string Name = name;
#define MODULE_DEFAULT(state) public: bool IsEnabled = state;

namespace IW3SR
{
	/// <summary>
	/// Game module.
	/// </summary>
	class Module
	{
		MODULE_INFO("id", "name");
		MODULE_DEFAULT(true);
	public:
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
