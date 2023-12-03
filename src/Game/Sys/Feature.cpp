#include "Feature.hpp"

namespace IW3SR
{
	Feature::Feature(const std::string& id, const std::string& name, const std::string& group)
	{
		ID = id;
		Name = name;
		Group = group;
		Menu = Window(name);
	}

	Feature::~Feature()
	{
		Shutdown();
	}

	void Feature::Initialize() { }
	void Feature::Shutdown() { }
	void Feature::OnMenu() { }
}
