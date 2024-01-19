#include "Feature.hpp"

namespace IW3SR
{
	Feature::Feature(const std::string& id, const std::string& name, const std::string& group)
		: Module(id, name, group) { }

	Feature::~Feature()
	{
		Release();
	}
}
