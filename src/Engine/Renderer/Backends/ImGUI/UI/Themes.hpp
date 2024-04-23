#pragma once
#include "ImGUI/Drawing/Window.hpp"

#include <tuple>

namespace IzEngine::UC
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
		/// Render frame.
		/// </summary>
		void Render();

	private:
		/// <summary>
		/// Default theme.
		/// </summary>
		void Default();

		/// <summary>
		/// Apply theme.
		/// </summary>
		void Apply();

		/// <summary>
		/// Compute rainbow color.
		/// </summary>
		void ComputeRainbow();

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

		SERIALIZE_POLY(Themes, Window, Style, PlotStyle)
	};
}
