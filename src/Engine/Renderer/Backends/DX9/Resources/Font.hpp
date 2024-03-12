#pragma once
#include "DX9/Base.hpp"

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
