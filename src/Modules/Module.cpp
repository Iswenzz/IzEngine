#include "Module.hpp"
#include <iostream>

namespace IW3SR
{
	Module::Module(const std::string& id, const std::string& name)
	{
		ID = id;
		Name = name;
		Menu = Window(name);
		Menu.SetRect(20, 20, 180, 80);
	}

	Module::~Module()
	{
		Shutdown();
	}

	void Module::Initialize() { }
	void Module::Shutdown() { }
	void Module::OnMenu() { }
	void Module::OnDraw3D() { }
	void Module::OnDraw2D() { }
	void Module::OnFrame() { }
}
