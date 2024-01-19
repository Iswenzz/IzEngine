#pragma once
#include "Engine/Core/Modules/Module.hpp"

namespace IW3SR
{
	/// <summary>
	/// Game feature.
	/// </summary>
	class API Feature : public Module
	{
	public:
		/// <summary>
		/// Initialize the feature.
		/// </summary>
		Feature() = default;

		/// <summary>
		/// Initialize the feature.
		/// </summary>
		/// <param name="id">The feature ID.</param>
		/// <param name="name">The feature name.</param>
		Feature(const std::string& id, const std::string& name, const std::string& group);

		/// <summary>
		/// Release the feature.
		/// </summary>
		virtual ~Feature();
	};
}
