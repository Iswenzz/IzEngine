#pragma once
#include "Engine/Backend/ImGUI/Common.hpp"

namespace IzEngine::UC
{
	// Asked once, and asked where the application says so rather than at startup: only it knows when
	// the client is far enough along to put a window in front of someone. Takes the menu over while
	// it is up, so it is the only thing on screen, and gives it back the moment it is answered.
	class Consent : public Frame
	{
	public:
		Consent();

		static void Prompt();

		void Initialize() override;
		void OnEvent(Event& event) override;
		void OnRender() override;

	private:
		void Answer(bool allow);
		void Dismiss();
	};
}
