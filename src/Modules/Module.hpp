#pragma once
#include <string>
#include <memory>

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
		bool IsEnabled;

		/// <summary>
		/// Release the module.
		/// </summary>
		virtual ~Module();

		/// <summary>
		/// Initialize the module.
		/// </summary>
		virtual void Initialize();

		/// <summary>
		/// Shutdown the module.
		/// </summary>
		virtual void Shutdown();

		/// <summary>
		/// Render frame callback.
		/// </summary>
		virtual void OnFrame();

		/// <summary>
		/// Draw 2D callback.
		/// </summary>
		virtual void OnDraw();
	};
}
