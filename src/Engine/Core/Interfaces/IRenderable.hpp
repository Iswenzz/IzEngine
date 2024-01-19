#pragma once
#include "IObject.hpp"

namespace IW3SR
{
	/// <summary>
	/// Renderable object.
	/// </summary>
	class API IRenderable : public IObject
	{
	public:
		/// <summary>
		/// Initialize renderable.
		/// </summary>
		virtual void Initialize() = 0;

		/// <summary>
		/// Release renderable.
		/// </summary>
		virtual void Release() = 0;

	protected:
		/// <summary>
		/// Initialize renderable.
		/// </summary>
		IRenderable() = default;

		/// <summary>
		/// Release renderable.
		/// </summary>
		virtual ~IRenderable() = default;
	};
}
