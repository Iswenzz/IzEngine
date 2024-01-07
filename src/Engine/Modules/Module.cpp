#include "Module.hpp"

namespace IW3SR
{
	Module::Module(const std::string& id, const std::string& name, const std::string& group)
	{
		ID = id;
		Name = name;
		Group = group;
		Menu = Window(name);
	}

	Module::~Module()
	{
		Shutdown();
	}

	void Module::Initialize() { }
	void Module::Shutdown() { }

	void Module::OnFinishMove(usercmd_s* cmd) { }
	void Module::OnMenu() { }
	void Module::OnDraw3D() { }
	void Module::OnDraw2D() { }
	void Module::OnFrame() { }
}
