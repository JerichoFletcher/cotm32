#include "drawers/csr_drawer.hpp"

#include "cotm32_defs.hpp"

static const char* reg_names[NUM_CSR] = {
  "mtvec",    "mepc",     "mcause",   "mtval"
};

CsrDrawer::CsrDrawer(const VerilatedContainer& v): m_v(v), m_csr(v) {}

void CsrDrawer::draw() {
  if (ImGui::Begin("CSR View", nullptr,
    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize
  )) {
    ImGui::BeginTable("csrs", 3,
      ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg
    );
    
    ImGui::TableSetupColumn("CSR");
    ImGui::TableSetupColumn("Dec");
    ImGui::TableSetupColumn("Hex");
    ImGui::TableHeadersRow();

    for (int i = 0; i < NUM_CSR; i++) {
      ImGui::TableNextRow();
      ImGui::TableNextColumn();
      ImGui::Text("%s", reg_names[i]);

      ImGui::TableNextColumn();
      ImGui::Text("%11d", (int32_t)this->m_csr[i]);

      ImGui::TableNextColumn();
      ImGui::Text("0x%08x", this->m_csr[i]);
    }

    ImGui::EndTable();
  }

  ImGui::End();
}
