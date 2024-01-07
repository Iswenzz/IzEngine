#pragma once
#include "DLL.hpp"

namespace IW3SR
{
	/// <summary>
	/// Dynamic Link Libraries.
	/// </summary>
	class DLLS
	{
	public:
		static inline std::map<std::string, std::unique_ptr<DLL>> Modules;
		static inline bool IsReloading = false;

		/// <summary>
		/// Initialize the DLLs.
		/// </summary>
		DLLS() = default;
		~DLLS();

		/// <summary>
		/// Initialize the modules.
		/// </summary>
		void Initialize();

		/// <summary>
		/// Reload the modules.
		/// </summary>
		void Reload();

		/// <summary>
		/// Compile the modules.
		/// </summary>
		void Compile();

		/// <summary>
		/// Set the dynamic modules renderer.
		/// </summary>
		void SetRenderer();
	};
}
