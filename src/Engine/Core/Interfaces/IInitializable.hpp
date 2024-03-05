#pragma once
#include "IObject.hpp"

namespace IW3SR::Engine
{
	/// <summary>
	/// Initializable object.
	/// </summary>
	class API IInitializable : public IObject
	{
	public:
		/// <summary>
		/// Initialize object.
		/// </summary>
		virtual void Initialize() = 0;

		/// <summary>
		/// Release object.
		/// </summary>
		virtual void Release() = 0;

	protected:
		/// <summary>
		/// Initialize object.
		/// </summary>
		IInitializable() = default;

		/// <summary>
		/// Release object.
		/// </summary>
		virtual ~IInitializable() = default;
	};
}
