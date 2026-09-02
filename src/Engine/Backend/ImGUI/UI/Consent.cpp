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
		Open = !Crash::Answered();
		if (!Open)
			return;

		UI::Open = true;
		UI::Modal = this;
	}

	void Consent::OnEvent(Event& event)
	{
		if (Open)
			UI::Open = true;

		Frame::OnEvent(event);
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
			"later in settings.");
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
	}
}
