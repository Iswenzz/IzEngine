#include "UI.hpp"

#include "Drawing/Notification.hpp"
#include "UI/Memory.hpp"
#include "UI/Themes.hpp"

#include "Core/System/Environment.hpp"
#include "Core/System/System.hpp"

namespace IzEngine
{
	void UI::Initialize()
	{
		IZ_ASSERT(!Active, "UI already initialized.");

		Context = ImGui::CreateContext();
		PlotContext = ImPlot::CreateContext();
		InitializeContext();

		Environment::Load(Serialized, "ui.json");

		KeyOpen = Bind(Key_F10);
		if (Serialized.contains("KeyOpen"))
			KeyOpen = Serialized["KeyOpen"];

		Setup();

		Add<UC::Themes>();
		Add<UC::Memory>();

		Active = true;
	}

	void UI::InitializeContext()
	{
		ImGui::SetAllocatorFunctions(Allocator, Free, &Data);
		ImGui::SetCurrentContext(Context);
		ImPlot::SetImGuiContext(Context);
		ImPlot::SetCurrentContext(PlotContext);
	}

	void UI::Shutdown()
	{
		IZ_ASSERT(Active, "UI already shutdown.");

		ImGui::DestroyContext(Context);
		ImPlot::DestroyContext(PlotContext);

		for (const auto& [_, frame] : Frames)
		{
			frame->Serialize(Serialized[frame->Name]);
			frame->Release();
		}
		Serialized["KeyOpen"] = KeyOpen;
		Environment::Save(Serialized, "ui.json");
		Frames.clear();

		Open = false;
		Active = false;
	}

	void UI::CreateScreen(const vec2& position, const vec2& size, const vec2& display)
	{
		Screen = VirtualScreen(position, size, display);
		Size = Screen.VirtualToReal.y * Scale;
	}

	double UI::Time()
	{
		return GImGui->Time;
	}

	float UI::DeltaTime()
	{
		return ImGui::GetIO().DeltaTime;
	}

	int UI::DeltaTimeMS()
	{
		return ImGui::GetIO().DeltaTime * 1000;
	}

	void* UI::Allocator(size_t size, void* data)
	{
		return malloc(size);
	}

	void UI::Free(void* ptr, void* data)
	{
		free(ptr);
	}

	void UI::Begin()
	{
		ImGui::NewFrame();
		ImGui::ComputeRainbow();
		Notifications::Render();

		if (Input::IsPressed(Key_Escape))
			Open = false;

		if (KeyOpen.IsPressed())
			Open = !Open;

		ImGui::GetIO().MousePos = Mouse::Position;
		ImGui::GetIO().MouseDrawCursor = UI::Open;
	}

	void UI::End()
	{
		ImGui::Render();
	}

	void UI::Dispatch(Event& event)
	{
		for (const auto& [_, frame] : Frames)
			frame->OnEvent(event);
	}

	void UI::Setup()
	{
		ImGuiIO& io = ImGui::GetIO();
		io.IniFilename = nullptr;

		const auto openSans = Environment::Path(Directory::Fonts) / "OpenSans-Regular.ttf";
		const auto faRegular = Environment::Path(Directory::Fonts) / "fa-regular-400.ttf";
		const auto faSolid = Environment::Path(Directory::Fonts) / "fa-solid-900.ttf";
		const auto faBrands = Environment::Path(Directory::Fonts) / "fa-brands-400.ttf";

		const float fontSize = 12.f * UI::Size;
		const float iconSize = 8.f * UI::Size;

		static const ImWchar rangesFa[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
		static const ImWchar rangesFab[] = { ICON_MIN_FAB, ICON_MAX_FAB, 0 };

		ImFontConfig iconConfig;
		iconConfig.MergeMode = true;
		iconConfig.PixelSnapH = true;
		iconConfig.OversampleH = true;
		iconConfig.GlyphMinAdvanceX = iconSize;

		io.Fonts->Clear();
		if (std::filesystem::exists(openSans))
			io.Fonts->AddFontFromFileTTF(openSans.string().c_str(), fontSize);
		if (std::filesystem::exists(faRegular))
			io.Fonts->AddFontFromFileTTF(faRegular.string().c_str(), iconSize, &iconConfig, rangesFa);
		if (std::filesystem::exists(faSolid))
			io.Fonts->AddFontFromFileTTF(faSolid.string().c_str(), iconSize, &iconConfig, rangesFa);
		if (std::filesystem::exists(faBrands))
			io.Fonts->AddFontFromFileTTF(faBrands.string().c_str(), iconSize, &iconConfig, rangesFab);
		if (std::filesystem::exists(openSans))
		{
			ImGui::H1 = io.Fonts->AddFontFromFileTTF(openSans.string().c_str(), fontSize * 1.5);
			ImGui::H2 = io.Fonts->AddFontFromFileTTF(openSans.string().c_str(), fontSize * 1.25);
			ImGui::H3 = io.Fonts->AddFontFromFileTTF(openSans.string().c_str(), fontSize * 1.125);
		}
		ImGui::MarkConfig.linkIcon = ICON_FA_LINK;
		ImGui::MarkConfig.linkCallback = MarkdownLink;
		ImGui::MarkConfig.imageCallback = MarkdownImage;
		ImGui::MarkConfig.formatCallback = MarkdownFormat;
		ImGui::MarkConfig.tooltipCallback = nullptr;
		ImGui::MarkConfig.headingFormats[0] = { ImGui::H1, true };
		ImGui::MarkConfig.headingFormats[1] = { ImGui::H2, true };
		ImGui::MarkConfig.headingFormats[2] = { ImGui::H3, false };
		ImGui::MarkConfig.userData = nullptr;
	}

	void UI::MarkdownLink(ImGui::MarkdownLinkCallbackData data)
	{
		std::string url(data.link, data.linkLength);
		if (!data.isImage)
			System::Shell(url);
	}

	ImGui::MarkdownImageData UI::MarkdownImage(ImGui::MarkdownLinkCallbackData data)
	{
		ImTextureID image = ImGui::GetIO().Fonts->TexID;
		ImGui::MarkdownImageData imageData;
		imageData.isValid = true;
		imageData.useLinkCallback = false;
		imageData.user_texture_id = image;
		imageData.size = ImVec2(40.0f, 20.0f);

		ImVec2 const contentSize = ImGui::GetContentRegionAvail();
		if (imageData.size.x > contentSize.x)
		{
			const float ratio = imageData.size.y / imageData.size.x;
			imageData.size.x = contentSize.x;
			imageData.size.y = contentSize.x * ratio;
		}
		return imageData;
	}

	void UI::MarkdownFormat(const ImGui::MarkdownFormatInfo& info, bool start)
	{
		ImGui::defaultMarkdownFormatCallback(info, start);

		switch (info.type)
		{
		case ImGui::MarkdownFormatType::LINK:
			start ? ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_ButtonActive])
				  : ImGui::PopStyleColor();
			break;
		}
	}
}
