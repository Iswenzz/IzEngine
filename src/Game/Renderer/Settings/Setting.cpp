#include "Setting.hpp"

namespace IW3SR::Game
{
	Setting::Setting(const std::string& id, const std::string& group, const std::string& name)
	{
		ID = id;
		Name = name;
		Group = group;
		Menu = Window(name);
	}

	Setting::~Setting()
	{
		Release();
	}

	void Setting::Initialize() { }
	void Setting::Release() { }
	void Setting::OnMenu() { }

	void Setting::OnDraw3D() { }
	void Setting::OnDraw3D(GfxCmdBufInput* cmd, GfxViewInfo* viewInfo, GfxCmdBufSourceState* src, GfxCmdBufState* buf) { }
	void Setting::OnDraw2D() { }
	void Setting::OnRender() { }
}
