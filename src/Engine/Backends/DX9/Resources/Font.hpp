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
		/// Create a font.
		/// </summary>
		Font() = default;
		~Font();
	};
}
