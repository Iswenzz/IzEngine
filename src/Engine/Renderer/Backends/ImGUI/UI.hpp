#pragma once
#include "ImGUI/Base.hpp"

#include "Drawing/Frame.hpp"

#include "Core/Input/Bind.hpp"
#include "Core/Input/Keyboard.hpp"
#include "Core/Input/Mouse.hpp"
#include "Core/Screen/VirtualScreen.hpp"

namespace IzEngine
{
	/// <summary>
	/// UI class.
	/// </summary>
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

		/// <summary>
		/// Initialize UI.
		/// </summary>
		static void Initialize();

		/// <summary>
		/// Initialize context.
		/// </summary>
		/// <remarks>Share context with DLLs.</remarks>
		static void InitializeContext();

		/// <summary>
		/// Initialize fonts.
		/// </summary>
		static void Setup();

		/// <summary>
		/// Release UI.
		/// </summary>
		static void Shutdown();

		/// <summary>
		/// Add a frame.
		/// </summary>
		/// <typeparam name="T">The frame class.</typeparam>
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

		/// <summary>
		/// Get time.
		/// </summary>
		/// <returns></returns>
		static double Time();

		/// <summary>
		/// Get delta time.
		/// </summary>
		/// <returns></returns>
		static float DeltaTime();

		/// <summary>
		/// Get delta time in milliseconds.
		/// </summary>
		/// <returns></returns>
		static int DeltaTimeMS();

		/// <summary>
		/// Begin frame.
		/// </summary>
		static void Begin();

		/// <summary>
		/// End frame.
		/// </summary>
		static void End();

		/// <summary>
		/// Resize UI.
		/// </summary>
		/// <param name="size">The size.</param>
		static void Resize(const vec2& size);

		/// <summary>
		/// Dispatch event.
		/// </summary>
		/// <param name="event">The event.</param>
		static void Dispatch(Event& event);

	private:
		static inline void* Data = nullptr;
		static inline ImGuiContext* Context = nullptr;
		static inline ImPlotContext* PlotContext = nullptr;
		static inline float Scale = 1.0f;

		/// <summary>
		/// ImGUI allocator.
		/// </summary>
		/// <param name="size">The size.</param>
		/// <param name="data">The data.</param>
		/// <returns></returns>
		static void* Allocator(size_t size, void* data);

		/// <summary>
		/// ImGUI free.
		/// </summary>
		/// <param name="ptr">The pointer.</param>
		/// <param name="data">The data.</param>
		static void Free(void* ptr, void* data);

		/// <summary>
		/// Markdown link callback.
		/// </summary>
		/// <param name="data">The data.</param>
		static void MarkdownLink(ImGui::MarkdownLinkCallbackData data);

		/// <summary>
		/// Markdown image callback.
		/// </summary>
		/// <param name="data">The data.</param>
		/// <returns></returns>
		static ImGui::MarkdownImageData MarkdownImage(ImGui::MarkdownLinkCallbackData data);

		/// <summary>
		/// Markdown format callback.
		/// </summary>
		/// <param name="info">The format info.</param>
		/// <param name="start">Is the start of the token.</param>
		static void MarkdownFormat(const ImGui::MarkdownFormatInfo& info, bool start);
	};
}
