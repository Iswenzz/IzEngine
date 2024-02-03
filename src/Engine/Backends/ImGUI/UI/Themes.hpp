#pragma once
#include "Engine/Backends/ImGUI/Components.hpp"
#include "Engine/Backends/ImGUI/Drawing/Window.hpp"

#include <tuple>

namespace IW3SR::Engine::UC
{
	/// <summary>
	/// Themes window.
	/// </summary>
	class Themes : public Window
	{
	public:
		ImGuiStyle Style;
		ImPlotStyle PlotStyle;
		std::tuple<ImColor, ImColor> Rainbow;
		ImGui::MarkdownConfig Markdown;

		ImFont* H1 = nullptr;
		ImFont* H2 = nullptr;
		ImFont* H3 = nullptr;

		/// <summary>
		/// Initialize the themes window.
		/// </summary>
		Themes();
		virtual ~Themes() = default;

		/// <summary>
		/// Initialize themes.
		/// </summary>
		void Initialize();

		/// <summary>
		/// Default theme.
		/// </summary>
		void Default();

		/// <summary>
		/// Compute rainbow color.
		/// </summary>
		void ComputeRainbow();

		/// <summary>
		/// Render frame.
		/// </summary>
		void Render();

	private:
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

		NLOHMANN_SERIALIZE_POLY(Themes, Window, Style, PlotStyle)
	};
}
