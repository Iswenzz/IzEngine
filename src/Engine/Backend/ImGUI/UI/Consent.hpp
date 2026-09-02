#pragma once
#include "Engine/Backend/ImGUI/Common.hpp"

namespace IzEngine::UC
{
	// Asked once, on the first launch rather than after the first crash: the handler captures from
	// startup, and an unanswered prompt means the dump sits in the database instead of being sent.
	// Takes the menu over while it is up, so it is the only thing on screen, and gives it back the
	// moment it is answered.
	class Consent : public Frame
	{
	public:
		Consent();

		void Initialize() override;
		void OnEvent(Event& event) override;
		void OnRender() override;

	private:
		void Answer(bool allow);
		void Dismiss();
	};
}
