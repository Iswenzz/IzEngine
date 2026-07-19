#include "UI.hpp"

#include "Memory.hpp"
#include "Themes.hpp"

#include "Engine/Backend/ImGUI/Common.hpp"

#include "Engine/Core/IO/VFS.hpp"
#include "Engine/Core/System/Environment.hpp"
#include "Engine/Core/System/System.hpp"
#include "Engine/Core/System/Window.hpp"

namespace IzEngine
{
	void UI::Initialize()
	{
		IZ_ASSERT(!Active, "UI already initialized.");
		IZ_ASSERT(Window::Handle, "Window is not initialized.");

		Screen = VirtualScreen(Window::Size);
		Size = Screen.VirtualToReal.y * Scale;

		Context = ImGui::CreateContext();
		PlotContext = ImPlot::CreateContext();
		UpdateContext();

		Environment::Load(Serialized, "ui.json");

		KeyOpen = Bind(Key_F10);
		if (Serialized.contains("KeyOpen"))
			KeyOpen = Serialized["KeyOpen"];

		Add<UC::Themes>();
		Add<UC::Memory>();

		Setup();

		Active = true;
	}

	void UI::UpdateContext()
	{
		ImGui::SetAllocatorFunctions(Allocator, Free, &Data);
		ImGui::SetCurrentContext(Context);
		ImPlot::SetImGuiContext(Context);
		ImPlot::SetCurrentContext(PlotContext);
	}

	void UI::Setup()
	{
		ImGuiIO& io = ImGui::GetIO();
		io.IniFilename = nullptr;

		static auto openSans = VFS::GetFile("Fonts/OpenSans-Regular.ttf");
		static auto faRegular = VFS::GetFile("Fonts/fa-regular-400.ttf");
		static auto faSolid = VFS::GetFile("Fonts/fa-solid-900.ttf");
		static auto faBrands = VFS::GetFile("Fonts/fa-brands-400.ttf");

		ImGui::GetStyle().FontScaleMain = Size / 2;
		const float fontSize = 16;
		const float iconSize = fontSize * 0.8;

		static ImFontConfig config;
		config.FontDataOwnedByAtlas = false;

		static ImFontConfig iconConfig;
		iconConfig.MergeMode = true;
		iconConfig.GlyphMinAdvanceX = iconSize;
		iconConfig.FontDataOwnedByAtlas = false;

		io.Fonts->Clear();
		io.Fonts->AddFontFromMemoryTTF(openSans.Data.data(), openSans.Data.size(), 0, &config);
		io.Fonts->AddFontFromMemoryTTF(faRegular.Data.data(), faRegular.Data.size(), iconSize, &iconConfig);
		io.Fonts->AddFontFromMemoryTTF(faSolid.Data.data(), faSolid.Data.size(), iconSize, &iconConfig);
		io.Fonts->AddFontFromMemoryTTF(faBrands.Data.data(), faBrands.Data.size(), iconSize, &iconConfig);

		ImGui::H1 = io.Fonts->AddFontFromMemoryTTF(openSans.Data.data(), openSans.Data.size(), fontSize * 4, &config);
		ImGui::H2 = io.Fonts->AddFontFromMemoryTTF(openSans.Data.data(), openSans.Data.size(), fontSize * 3, &config);
		ImGui::H3 = io.Fonts->AddFontFromMemoryTTF(openSans.Data.data(), openSans.Data.size(), fontSize * 2, &config);

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

	void UI::Shutdown()
	{
		IZ_ASSERT(Active, "UI already shutdown.");

		for (const auto& [_, frame] : Frames)
		{
			frame->Serialize(Serialized[frame->Name]);
			frame->Release();
		}
		Serialized["KeyOpen"] = KeyOpen;
		Environment::Save(Serialized, "ui.json");
		Frames.clear();

		UpdateContext();
		ImGui::DestroyContext(Context);
		ImPlot::DestroyContext(PlotContext);
		Context = nullptr;
		PlotContext = nullptr;

		Open = false;
		Active = false;
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

	void UI::Resize(const vec2& size)
	{
		Screen = VirtualScreen(size);
		Size = Screen.VirtualToReal.y * Scale;

		if (Active)
		{
			Open = false;
			ImGui::GetStyle().FontScaleMain = Size / 2;
		}
	}

	void UI::Dispatch(Event& event)
	{
		for (const auto& [_, frame] : Frames)
			frame->OnEvent(event);
	}

	void UI::OpenMenu()
	{
		Open = true;
	}

	void UI::OpenWindow(const std::string& name)
	{
		Frames[name]->Open = true;
	}

	void UI::MarkdownLink(ImGui::MarkdownLinkCallbackData data)
	{
		std::string url(data.link, data.linkLength);
		if (!data.isImage)
			System::Shell(url);
	}

	ImGui::MarkdownImageData UI::MarkdownImage(ImGui::MarkdownLinkCallbackData data)
	{
		ImTextureRef image = ImGui::GetIO().Fonts->TexID;
		ImGui::MarkdownImageData imageData;
		imageData.isValid = true;
		imageData.useLinkCallback = false;
		imageData.user_texture_id = image.GetTexID();
		imageData.size = { 40.0f, 20.0f };

		const auto contentSize = ImGui::GetContentRegionAvail();
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
