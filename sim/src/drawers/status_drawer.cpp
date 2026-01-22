#include "drawers/status_drawer.hpp"

#include "colors.hpp"
#include "drawers/draw_utils.hpp"
#include "imgui.h"
#include "name_utils.hpp"
#include "views/status_view.hpp"

using namespace cotm32::colors;
using cotm32::draw_utils::draw_signal;
using cotm32::name_utils::priv_mode_name;

void StatusDrawer::render(const Simulator& sim) {
    if (ImGui::BeginChild(
            "status", ImVec2(ImGui::GetContentRegionAvail().x, 0), ImGuiChildFlags_AutoResizeY
        )) {
        if (ImGui::CollapsingHeader("Status", ImGuiTreeNodeFlags_DefaultOpen)) {
            StatusView view(sim.v());

            draw_signal("M-mode interrupt enable", view.m_interr_enable(), GREEN, OFF, false);
            auto& priv_name = priv_mode_name(view.privilege());
            if (!priv_name.empty()) {
                ImGui::Text("Priv   : %s", priv_name.c_str());
            } else {
                ImGui::Text("Priv   : ??? (%d)", view.privilege());
            }
        }
    }
    ImGui::EndChild();
}
