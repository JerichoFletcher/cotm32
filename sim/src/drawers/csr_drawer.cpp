#include "drawers/csr_drawer.hpp"

#include "cotm32_defs.hpp"
#include "imgui.h"
#include "name_utils.hpp"
#include "views/csr_view.hpp"

void CsrDrawer::render(const Simulator& sim) {
    if (ImGui::BeginChild(
            "csr", ImVec2(ImGui::GetContentRegionAvail().x, 0), ImGuiChildFlags_AutoResizeY
        )) {
        if (ImGui::CollapsingHeader("CSR View", ImGuiTreeNodeFlags_DefaultOpen)) {
            if (ImGui::BeginTable(
                    "table_csr", 3, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_RowBg
                )) {
                ImGui::TableSetupColumn("CSR", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableSetupColumn("Dec");
                ImGui::TableSetupColumn("Hex");
                ImGui::TableHeadersRow();

                CsrView csr(sim.v());

                for (int i = 0; i < NUM_CSR; i++) {
                    auto csr_id = CSR_IDS[i];

                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    auto csr_name = cotm32::name_utils::csr_name(csr_id);
                    ImGui::Text("%s", !csr_name.empty() ? csr_name.c_str() : "???");

                    ImGui::TableNextColumn();
                    ImGui::Text("%11d", (int32_t)csr[csr_id]);

                    ImGui::TableNextColumn();
                    ImGui::Text("0x%08x", csr[csr_id]);
                }

                ImGui::EndTable();
            }
        }
    }
    ImGui::EndChild();
}
