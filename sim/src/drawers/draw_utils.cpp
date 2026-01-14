#include "drawers/draw_utils.hpp"

static constexpr inline int SIG_RADIUS = 6.0f;

namespace cotm32::draw_utils {
void draw_signal(
    const char* label, bool on, ImU32 on_color, ImU32 off_color, bool use_avail_region
) {
    auto* draw_list = ImGui::GetWindowDrawList();
    auto pos = ImGui::GetCursorScreenPos();
    auto col = on ? on_color : off_color;

    float y = pos.y + (use_avail_region ? ImGui::GetContentRegionAvail().y / 2
                                        : ImGui::GetTextLineHeight() / 2);
    draw_list->AddCircleFilled(ImVec2(pos.x + SIG_RADIUS, y), SIG_RADIUS, col);
    ImGui::Dummy(ImVec2(SIG_RADIUS * 2, SIG_RADIUS * 2));
    ImGui::SameLine();
    ImGui::TextUnformatted(label);
}

void draw_signal(
    const std::string& label, bool on, ImU32 on_color, ImU32 off_color, bool use_avail_region
) {
    draw_signal(label.c_str(), on, on_color, off_color, use_avail_region);
}
}  // namespace cotm32::draw_utils
