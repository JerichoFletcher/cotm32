#include "drawers/reg_drawer.hpp"

#include "cotm32_defs.hpp"
#include "imgui.h"
#include "name_utils.hpp"
#include "views/reg_view.hpp"

void RegDrawer::render(const Simulator& sim) {
    if (ImGui::BeginChild(
            "reg", ImVec2(ImGui::GetContentRegionAvail().x, 0), ImGuiChildFlags_AutoResizeY
        )) {
        if (ImGui::CollapsingHeader("Register View", ImGuiTreeNodeFlags_DefaultOpen)) {
            if (ImGui::BeginTable(
                    "table_reg", 3, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_RowBg
                )) {
                ImGui::TableSetupColumn("Reg", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableSetupColumn("Dec");
                ImGui::TableSetupColumn("Hex");
                ImGui::TableHeadersRow();

                RegView reg(sim.v());

                for (int i = 0; i < NUM_REGS; i++) {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text("%s", cotm32::name_utils::reg_name(i).c_str());

                    ImGui::TableNextColumn();
                    ImGui::Text("%11d", (int32_t)reg[i]);

                    ImGui::TableNextColumn();
                    ImGui::Text("0x%08x", reg[i]);
                }

                ImGui::EndTable();
            }
        }
    }
    ImGui::EndChild();
}
