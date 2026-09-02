#pragma once
#include "Engine/Backend/ImGUI/Common.hpp"

namespace IzEngine::UC
{
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
