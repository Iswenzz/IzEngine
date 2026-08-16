#pragma once
#include "Engine/Backend/ImGUI/Base.hpp"
#include "Engine/Backend/ImGUI/Components/Frame.hpp"

#include "Engine/Core/Input/Bind.hpp"
#include "Engine/Core/Input/Keyboard.hpp"
#include "Engine/Core/Input/Mouse.hpp"

namespace IzEngine
{
	class API UI
	{
	public:
		static inline std::unordered_map<std::string, Ref<Frame>> Frames;
		static inline nlohmann::json Serialized;
		static inline Bind KeyOpen;

		static inline bool Active = false;
		static inline bool Open = false;
		static inline bool DesignMode = false;
		static inline VirtualScreen Screen;
		static inline float Size = 1.0f;

		static void Initialize();
		static void UpdateContext();
		static void Setup();
		static void Shutdown();

		static double Time();
		static float DeltaTime();
		static int DeltaTimeMS();

		static void Begin();
		static void End();
		static void Resize(const vec2& size);
		static void Dispatch(Event& event);

		static void OpenMenu();
		static void OpenWindow(const std::string& name);
		static Ref<Frame> GetWindow(const std::string& name);
		static bool* GetWindowState(const std::string& name);

		template <class T = Frame>
		static void Add()
		{
			auto frame = CreateRef<T>();
			bool isSerialized = Serialized.contains(frame->Name);

			if (isSerialized)
				frame->Deserialize(Serialized[frame->Name]);
			frame->Initialize();

			Frames[frame->Name] = frame;
		}

	private:
		static inline void* Data = nullptr;
		static inline ImGuiContext* Context = nullptr;
		static inline ImPlotContext* PlotContext = nullptr;
		static inline float Scale = 1.0f;

		static void* Allocator(size_t size, void* data);
		static void Free(void* ptr, void* data);

		static void MarkdownLink(ImGui::MarkdownLinkCallbackData data);
		static ImGui::MarkdownImageData MarkdownImage(ImGui::MarkdownLinkCallbackData data);
		static void MarkdownFormat(const ImGui::MarkdownFormatInfo& info, bool start);
	};
}
