#include "Reporter.hpp"

#include <thread>

namespace IzEngine
{
	// Logical sizes at 96 dpi.
	constexpr vec2 WindowSize = { 720, 660 };
	constexpr float Gap = 8.0f;
	constexpr float DescriptionHeight = 90.0f;
	constexpr float LabelWidth = 110.0f;

	constexpr vec4 ColorBackground = { 0.09f, 0.09f, 0.11f, 1.00f };
	constexpr vec4 ColorPanel = { 0.05f, 0.05f, 0.06f, 1.00f };
	constexpr vec4 ColorBody = { 0.75f, 0.75f, 0.78f, 1.00f };
	constexpr vec4 ColorError = { 0.92f, 0.43f, 0.39f, 1.00f };
	constexpr vec4 ColorIcon = { 0.90f, 0.30f, 0.28f, 1.00f };
	constexpr vec4 ColorAccent = { 0.16f, 0.44f, 0.75f, 1.00f };
	constexpr vec4 ColorAccentHovered = { 0.22f, 0.52f, 0.84f, 1.00f };
	constexpr vec4 ColorAccentActive = { 0.12f, 0.36f, 0.64f, 1.00f };

	Reporter::Reporter(CrashReport& report, float scale) : Frame("Crash Reporter"), Current(&report), Scale(scale)
	{
		Open = true;
	}

	int Reporter::Run(CrashReport& report)
	{
		const float scale = Window::GetDisplayScale();

		Window::Size = WindowSize * scale;
		Window::Initialize("Crash Reporter");
		Window::SetDarkMode(true);

		if (!Renderer::Initialize(RendererBackend::DX9))
		{
			Window::Shutdown();
			Fallback(report);
			return 0;
		}

		// The engine greets the player on its first frame; there is no one to greet here.
		Notifications::Clear();

		Reporter reporter(report, scale);
		UI::Modal = &reporter;
		UI::OpenMenu();

		while (Window::Frame() && !reporter.Closed)
		{
			// The UI sizes its font from the window height, which suits a game filling the screen;
			// a dialog wants the display's own scale, and a resize puts the height back.
			ImGui::GetStyle().FontScaleMain = scale;
			Renderer::Frame();
			Input::Reset();
			reporter.Poll();
		}

		// The title bar can close the window mid-upload; the answer still lands on this object.
		while (reporter.Sending)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
			reporter.Poll();
		}

		UI::Modal = nullptr;
		Renderer::Shutdown();
		Window::Shutdown();
		return 0;
	}

	void Reporter::Fallback(const CrashReport& report)
	{
		System::Alert("Crash Reporter",
			report.Application + " has crashed, and the crash window could not be shown.\n\n"
			"The crash report was saved to:\n" + report.DumpPath.string() + "\n\n" + report.Details);
	}

	void Reporter::OnRender()
	{
		constexpr ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize
			| ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings
			| ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse
			| ImGuiWindowFlags_NoBringToFrontOnFocus;

		// The overlay draws its own cursor over the game; a dialog keeps the system one.
		ImGui::GetIO().MouseDrawCursor = false;

		ImGui::SetNextWindowPos(vec2(0, 0));
		ImGui::SetNextWindowSize(Window::Size);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, vec2(24, 24) * Scale);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ColorBackground);

		if (ImGui::Begin("##CrashReporter", nullptr, flags))
			Content();
		ImGui::End();

		ImGui::PopStyleColor();
		ImGui::PopStyleVar(3);

		if (!Sending && ImGui::IsKeyPressed(ImGuiKey_Escape, false))
			Closed = true;
	}

	void Reporter::Content()
	{
		const float gap = Gap * Scale;
		const bool sendable = Current->Endpoint.Valid;

		ImGui::PushFont(nullptr, 34.0f);
		ImGui::PushStyleColor(ImGuiCol_Text, ColorIcon);
		ImGui::TextUnformatted(ICON_FA_CIRCLE_XMARK);
		ImGui::PopStyleColor();
		ImGui::PopFont();

		ImGui::SameLine(0, gap * 2);
		ImGui::PushFont(nullptr, 26.0f);
		ImGui::TextUnformatted((Current->Application + " has crashed").c_str());
		ImGui::PopFont();

		ImGui::Dummy(vec2(0, gap));
		ImGui::PushStyleColor(ImGuiCol_Text, ColorBody);
		ImGui::TextWrapped(sendable
				? "Sorry about that. A crash report has been written. Sending it helps find and fix the cause: it "
				  "contains a memory snapshot of the crashed process and basic details about your system. Nothing is "
				  "sent unless you click Send."
				: "Sorry about that. A crash report has been written to the reports folder. This build has nowhere "
				  "to send it, so share the dump file with the developers if you can.");
		ImGui::PopStyleColor();
		ImGui::Dummy(vec2(0, gap * 2));

		ImGui::TextDisabled("What were you doing when it crashed? (optional)");
		ImGui::InputTextMultiline("##description", &Description, vec2(-FLT_MIN, DescriptionHeight * Scale),
			Sending ? ImGuiInputTextFlags_ReadOnly : 0);
		ImGui::Dummy(vec2(0, gap));

		ImGui::TextDisabled("Crash details");
		ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - ImGui::CalcTextSize("Copy").x
			- ImGui::GetStyle().FramePadding.x * 2);
		if (ImGui::SmallButton("Copy"))
			ImGui::SetClipboardText(Current->Details.c_str());

		// What is left above the status line and the buttons.
		const float footer = ImGui::GetTextLineHeightWithSpacing() + ImGui::GetFrameHeight() + gap * 3;
		Details(std::max(ImGui::GetContentRegionAvail().y - footer, 80.0f * Scale));
		ImGui::Dummy(vec2(0, gap));

		ImGui::PushStyleColor(ImGuiCol_Text, StatusError ? ImVec4(ColorError) : ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
		ImGui::TextUnformatted(Status.c_str());
		ImGui::PopStyleColor();
		ImGui::Dummy(vec2(0, gap));

		Buttons();
	}

	void Reporter::Details(float height)
	{
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ColorPanel);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, vec2(12, 8) * Scale);
		ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, vec2(6, 3) * Scale);
		ImGui::BeginChild("##details", vec2(-FLT_MIN, height), ImGuiChildFlags_Borders | ImGuiChildFlags_AlwaysUseWindowPadding);

		if (ImGui::BeginTable("##fields", 2, ImGuiTableFlags_SizingFixedFit))
		{
			ImGui::TableSetupColumn("label", ImGuiTableColumnFlags_WidthFixed, LabelWidth * Scale);
			ImGui::TableSetupColumn("value", ImGuiTableColumnFlags_WidthStretch);
			for (const auto& [label, value] : Current->Fields)
			{
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::TextDisabled("%s", label.c_str());
				ImGui::TableNextColumn();
				ImGui::TextWrapped("%s", value.c_str());
			}
			ImGui::EndTable();
		}

		ImGui::EndChild();
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor();
	}

	void Reporter::Buttons()
	{
		const float gap = Gap * Scale;
		const bool sendable = Current->Endpoint.Valid;
		const float height = ImGui::GetFrameHeight() + gap;
		const auto width = [&](const char* label)
		{ return ImGui::CalcTextSize(label).x + ImGui::GetStyle().FramePadding.x * 2 + 24.0f * Scale; };

		const char* folder = ICON_FA_FOLDER_OPEN "  Open Reports Folder";
		const char* close = sendable ? "Close without Sending" : "Close";
		const char* send = Failed ? "Retry" : "Send and Close";
		const float sendWidth = sendable ? width(send) + gap : 0.0f;

		ImGui::BeginDisabled(Sending);

		if (ImGui::Button(folder, vec2(width(folder), height)))
			System::Shell(Current->DumpPath.parent_path().string());

		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x - width(close) - sendWidth);
		if (ImGui::Button(close, vec2(width(close), height)))
			Closed = true;

		if (sendable)
		{
			ImGui::SameLine(0, gap);
			ImGui::PushStyleColor(ImGuiCol_Button, ColorAccent);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ColorAccentHovered);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ColorAccentActive);
			if (ImGui::Button(send, vec2(width(send), height)))
				StartSend();
			ImGui::PopStyleColor(3);
		}

		ImGui::EndDisabled();
	}

	void Reporter::StartSend()
	{
		Sending = true;
		Outcome = 0;
		Status = "Sending the report...";
		StatusError = false;

		Report::Send(*Current, Description,
			[this](bool success, const std::string& error)
			{
				{
					std::scoped_lock lock(Lock);
					Error = error;
				}
				Outcome = success ? 1 : 2;
			});
	}

	void Reporter::Poll()
	{
		const int outcome = Outcome.exchange(0);
		if (!outcome)
			return;

		Sending = false;
		if (outcome == 1)
		{
			Closed = true;
			return;
		}

		std::scoped_lock lock(Lock);
		Failed = true;
		Status = "Sending failed: " + Error;
		StatusError = true;
	}
}
