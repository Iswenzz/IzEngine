#include "Consent.hpp"

#include "Engine/Core/System/Crash.hpp"

namespace IzEngine::UC
{
	Consent::Consent() : Frame("Crash Reports")
	{
		SetRect(-150, -75, 300, 150);
		SetRectAlignment(Horizontal::Center, Vertical::Center);
		SetFlags(ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
			| ImGuiWindowFlags_NoSavedSettings);
	}

	void Consent::Initialize()
	{
		Open = false;
	}

	void Consent::Prompt()
	{
		if (Crash::Answered())
			return;

		const auto frame = std::dynamic_pointer_cast<Consent>(UI::GetWindow("Crash Reports"));
		if (!frame || frame->Open)
			return;

		frame->Open = true;
		UI::Open = true;
		UI::Modal = frame.get();
	}

	void Consent::Answer(bool allow)
	{
		Crash::Consent(allow);
		Dismiss();
	}

	void Consent::Dismiss()
	{
		Open = false;
		UI::Modal = nullptr;
		UI::Open = false;
	}

	void Consent::OnRender()
	{
		vec2 position = Position;
		vec2 size = Size;
		UI::Screen.Apply(position, size, HorizontalAlign, VerticalAlign);

		ImGui::SetNextWindowPos(position);
		ImGui::SetNextWindowSize(size);
		ImGui::Begin(Name.c_str(), nullptr, Flags);

		ImGui::TextWrapped("One question before you start, asked just this once.");
		ImGui::Spacing();

		ImGui::TextWrapped(
			"Games crash sometimes. When this one does, it writes down what it was "
			"doing just beforehand and what kind of computer it was running on. "
			"Should it send us that note?");
		ImGui::Spacing();

		ImGui::TextWrapped(
			"We ask because it is your computer and your choice. Without the note, a "
			"crash is nearly impossible to track down, since we cannot make it happen "
			"again on demand. With it, we usually find the cause quickly and fix it "
			"for everyone.");
		ImGui::Spacing();

		ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
		ImGui::TextWrapped(
			"The note is saved on your computer either way. You can change this "
			"later in settings, and Escape leaves the question for the next launch.");
		ImGui::PopStyleColor();

		const float height = ImGui::GetFrameHeight();
		ImGui::SetCursorPosY(ImGui::GetWindowHeight() - height - ImGui::GetStyle().WindowPadding.y);

		const float width = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x) * 0.5f;
		if (ImGui::Button("Send reports", { width, height }))
			Answer(true);

		ImGui::SameLine();
		if (ImGui::Button("Don't send", { width, height }))
			Answer(false);

		ImGui::End();

		// The prompt holds the overlay open, so a host that never gets its input as far as ImGui
		// leaves the buttons with nothing to click them. Escape hands the application back without
		// recording an answer either way, and the question returns on the next launch.
		if (Input::IsPressed(Key_Escape) || ImGui::IsKeyPressed(ImGuiKey_Escape, false))
			Dismiss();
	}
}
