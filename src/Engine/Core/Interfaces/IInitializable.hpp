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
		/// Initialize.
		/// </summary>
		virtual void Initialize() = 0;

		/// <summary>
		/// Release.
		/// </summary>
		virtual void Release() = 0;

	protected:
		/// <summary>
		/// Initialize.
		/// </summary>
		IInitializable() = default;

		/// <summary>
		/// Release.
		/// </summary>
		virtual ~IInitializable() = default;
	};
}
