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

	void Module::OnEvent(Event& event)
	{
		if (!IsEnabled) return;
		EventDispatcher dispatcher(event);

		dispatcher.Dispatch<EventRenderer3D>(EVENT_BIND(OnDraw3D));
		dispatcher.Dispatch<EventRenderer2D>(EVENT_BIND(OnDraw2D));
		dispatcher.Dispatch<EventRendererRender>(EVENT_BIND_VOID(OnRender));

		dispatcher.Dispatch<EventPlayerLoadPosition>(EVENT_BIND_VOID(OnLoadPosition));
		dispatcher.Dispatch<EventPMoveWalk>(EVENT_BIND(OnWalkMove));
		dispatcher.Dispatch<EventPMoveAir>(EVENT_BIND(OnAirMove));
		dispatcher.Dispatch<EventPMoveFinish>(EVENT_BIND(OnFinishMove));
	}

	void Module::OnMenu() { }

	void Module::OnLoadPosition() { }
	void Module::OnWalkMove(EventPMoveWalk& event) { }
	void Module::OnAirMove(EventPMoveAir& event) { }
	void Module::OnFinishMove(EventPMoveFinish& event) { }

	void Module::OnDraw3D(EventRenderer3D& event) { }
	void Module::OnDraw2D(EventRenderer2D& event) { }
	void Module::OnRender() { }
}
