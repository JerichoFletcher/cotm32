#include "drawers/csr_drawer.hpp"

#include "imgui.h"

#include "cotm32_defs.hpp"
#include "views/csr_view.hpp"

static const char* reg_names[NUM_CSR] = {
  "mtvec",    "mepc",     "mcause",   "mtval"
};

void CsrDrawer::render(const Simulator& sim) {
  if (ImGui::BeginChild("csr",
    ImVec2(ImGui::GetContentRegionAvail().x, 0),
    ImGuiChildFlags_AutoResizeY
  )) {
    if (ImGui::CollapsingHeader("CSR View", ImGuiTreeNodeFlags_DefaultOpen)) {
      if (ImGui::BeginTable("table_csr", 3,
        ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_RowBg
      )) {
        ImGui::TableSetupColumn("CSR", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Dec");
        ImGui::TableSetupColumn("Hex");
        ImGui::TableHeadersRow();
    
        CsrView csr(sim.v());

        for (int i = 0; i < NUM_CSR; i++) {
          ImGui::TableNextRow();
          ImGui::TableNextColumn();
          ImGui::Text("%s", reg_names[i]);
    
          ImGui::TableNextColumn();
          ImGui::Text("%11d", (int32_t)csr[i]);
    
          ImGui::TableNextColumn();
          ImGui::Text("0x%08x", csr[i]);
        }

        ImGui::EndTable();
      }
    }
  }
  ImGui::EndChild();
}
