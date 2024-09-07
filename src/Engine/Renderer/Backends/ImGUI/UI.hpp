#pragma once
#include "ImGUI/Base.hpp"

#include "Drawing/Window.hpp"

#include "Core/Input/Keyboard.hpp"
#include "Core/Screen/VirtualScreen.hpp"

namespace IzEngine
{
	/// <summary>
	/// UI class.
	/// </summary>
	class API UI
	{
	public:
		static inline std::unordered_map<std::string, Ref<Window>> Windows;
		static inline nlohmann::json Serialized;
		static inline Keyboard KeyOpen;

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
		/// Release UI.
		/// </summary>
		static void Shutdown();

		/// <summary>
		/// Load a module.
		/// </summary>
		/// <typeparam name="T">The module type.</typeparam>
		/// <param name="enabled">Is enabled by default.</param>
		template <class T = Window>
		static void CreateWindow()
		{
			auto window = CreateRef<T>();
			bool isSerialized = Serialized.contains(window->Name);

			if (isSerialized)
				window->Deserialize(Serialized[window->Name]);
			window->Initialize();

			Windows[window->Name] = window;
		}

		/// <summary>
		/// Create the screen.
		/// </summary>
		/// <param name="position">The position.</param>
		/// <param name="size">The size.</param>
		/// <param name="display">The display size.</param>
		static void CreateScreen(const vec2& position, const vec2& size, const vec2& display);

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
	};
}
