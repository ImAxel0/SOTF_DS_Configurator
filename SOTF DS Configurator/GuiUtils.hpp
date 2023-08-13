#include "imgui/imgui.h"
#include "Drawing.h"

inline bool showAbout = false;
inline bool showLog = false;
inline bool showPassword = false;

namespace Colors
{
	ImColor green = ImColor(66, 185, 131, 230);
	ImColor greenHov = ImColor(66-30, 185-30, 131-30, 230);
	ImColor lime = ImColor(124, 248, 104, 230);
	ImColor limeHov = ImColor(124-30, 248-30, 104-30, 230);
	ImColor red = ImColor(255, 65, 111, 230);
	ImColor redHov = ImColor(255-30, 65-30, 111-30, 230);
}

inline void PushButtonCol(ImColor color, ImColor hovered)
{
	Drawing::style->Colors[ImGuiCol_Button] = color;
	Drawing::style->Colors[ImGuiCol_ButtonHovered] = hovered;
}

inline void InfoBox(std::string info)
{
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_None))
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(info.c_str());
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

inline void TextCenteredXY(std::string text) {
	auto windowWidth = ImGui::GetContentRegionAvail().x;
	auto textWidth = ImGui::CalcTextSize(text.c_str()).x;

	auto windowHeight = ImGui::GetContentRegionAvail().y;
	auto textHeight = ImGui::CalcTextSize(text.c_str()).y;

	ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
	ImGui::SetCursorPosY((windowHeight - textHeight) * 0.5f);
	ImGui::Text(text.c_str());
}

inline void TextCentered(std::string text) {
	auto windowWidth = ImGui::GetWindowContentRegionWidth();
	auto textWidth = ImGui::CalcTextSize(text.c_str()).x;

	ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
	ImGui::Text(text.c_str());
}
