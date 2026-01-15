#include "drawers/clint_drawer.hpp"

#include "colors.hpp"
#include "cotm32_defs.hpp"
#include "drawers/draw_utils.hpp"
#include "name_utils.hpp"
#include "views/clint_view.hpp"

using cotm32::draw_utils::draw_signal;

void make_row(const char* name, uint64_t value, bool hex) {
    ImGui::TableNextRow();
    ImGui::TableNextColumn();
    ImGui::TextUnformatted(name);

    ImGui::TableNextColumn();
    if (hex) {
        ImGui::Text("0x%016lx", value);
    } else {
        ImGui::Text("%20ld", value);
    }
}

void ClintDrawer::render(const Simulator& sim) {
    if (ImGui::BeginChild(
            "clint", ImVec2(ImGui::GetContentRegionAvail().x, 0), ImGuiChildFlags_AutoResizeY
        )) {
        if (ImGui::CollapsingHeader("CLINT View", ImGuiTreeNodeFlags_DefaultOpen)) {
            ClintView view(sim.v());

            draw_signal(
                "Write Enable",
                view.write_enable(),
                cotm32::colors::GREEN,
                cotm32::colors::OFF,
                false
            );
            draw_signal(
                "Timer Interrupt Pending",
                view.mtip(),
                cotm32::colors::YELLOW,
                cotm32::colors::OFF,
                false
            );

            ImGui::Separator();
            draw_signal("msip", view.msip(), cotm32::colors::YELLOW, cotm32::colors::OFF, false);
            ImGui::Text("mtime       : %-20ld", view.mtime());
            ImGui::Text("mtimecmp    : %-20ld", view.mtimecmp());
        }
    }
    ImGui::EndChild();
}
