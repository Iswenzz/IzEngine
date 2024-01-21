#pragma once
#include "Engine/Backends/DX9.hpp"

namespace IW3SR::Engine
{
	/// <summary>
	/// Font class.
	/// </summary>
	class API Font : public IObject
	{
	public:
		ID3DXFont* Base = nullptr;

		/// <summary>
		/// Initialize a new Font.
		/// </summary>
		/// <param name="id">The font ID.</param>
		Font(const std::string& id);
		~Font();
	};
}
