#include "Module.hpp"

namespace IW3SR::Game
{
	Module::Module(const std::string& id, const std::string& group, const std::string& name)
	{
		ID = id;
		Name = name;
		Group = group;
		Menu = Window(name);
	}

	Module::~Module()
	{
		Release();
	}

	void Module::Initialize() { }
	void Module::Release() { }
	void Module::OnMenu() { }

	void Module::OnFinishMove(usercmd_s* cmd) { }
	void Module::OnWalkMove(pmove_t* pm, pml_t* pml) { }
	void Module::OnAirMove(pmove_t* pm, pml_t* pml) { }
	void Module::OnLoadPosition() { }

	void Module::OnDraw3D() { }
	void Module::OnDraw3D(GfxCmdBufInput* cmd, GfxViewInfo* viewInfo, GfxCmdBufSourceState* src, GfxCmdBufState* buf) { }
	void Module::OnDraw2D() { }
	void Module::OnRender() { }
}
