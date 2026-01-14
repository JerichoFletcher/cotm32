#include "drawers/reg_drawer.hpp"

#include "colors.hpp"
#include "cotm32_defs.hpp"
#include "drawers/draw_utils.hpp"
#include "imgui.h"
#include "name_utils.hpp"
#include "views/reg_view.hpp"

using cotm32::draw_utils::draw_signal;

void RegDrawer::render(const Simulator& sim) {
    if (ImGui::BeginChild(
            "reg", ImVec2(ImGui::GetContentRegionAvail().x, 0), ImGuiChildFlags_AutoResizeY
        )) {
        if (ImGui::CollapsingHeader("Register View", ImGuiTreeNodeFlags_DefaultOpen)) {
            RegView view(sim.v());
            draw_signal(
                "Write Enable",
                view.write_enable(),
                cotm32::colors::GREEN,
                cotm32::colors::OFF,
                false
            );

            if (ImGui::BeginTable(
                    "table_reg", 3, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_RowBg
                )) {
                ImGui::TableSetupColumn("Reg", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableSetupColumn("Dec");
                ImGui::TableSetupColumn("Hex");
                ImGui::TableHeadersRow();

                for (int i = 0; i < NUM_REGS; i++) {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text("%s", cotm32::name_utils::reg_name(i).c_str());

                    ImGui::TableNextColumn();
                    ImGui::Text("%11d", (int32_t)view[i]);

                    ImGui::TableNextColumn();
                    ImGui::Text("0x%08x", view[i]);
                }

                ImGui::EndTable();
            }
        }
    }
    ImGui::EndChild();
}
