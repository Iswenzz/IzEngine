#pragma once
#include "Engine/Backends/DX9.hpp"

namespace IW3SR
{
	/// <summary>
	/// Font class.
	/// </summary>
	class API Font
	{
	public:
		ComPtr<ID3DXFont> Base;

		/// <summary>
		/// Initialize a new Font.
		/// </summary>
		Font() = default;
		~Font() = default;

		/// <summary>
		/// Reset resources.
		/// </summary>
		void Reset();
	};
}
