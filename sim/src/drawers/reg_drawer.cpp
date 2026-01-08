#include "drawers/reg_drawer.hpp"

#include "cotm32_defs.hpp"

static const char* reg_names[NUM_REGS] = {
  "x0/zero",  "x1/ra",    "x2/sp",    "x3/gp",
  "x4/tp",    "x5/t0",    "x6/t1",    "x7/t2",
  "x8/s0",    "x9/s1",    "x10/a0",   "x11/a1",
  "x12/a2",   "x13/a3",   "x14/a4",   "x15/a5",
  "x16/a6",   "x17/a7",   "x18/s2",   "x19/s3",
  "x20/s4",   "x21/s5",   "x22/s6",   "x23/s7",
  "x24/s8",   "x25/s9",   "x26/s10",  "x27/s11",
  "x28/t3",   "x29/t4",   "x30/t5",   "x31/t6"
};

RegDrawer::RegDrawer(const VerilatedContainer& v): m_v(v), m_reg(v) {}

void RegDrawer::draw() {
  if (ImGui::BeginChild("reg",
    ImVec2(ImGui::GetContentRegionAvail().x, 0),
    ImGuiChildFlags_AutoResizeY
  )) {
    if (ImGui::CollapsingHeader("Register View")) {
      if (ImGui::BeginTable("table_reg", 3,
        ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg
      )) {
        ImGui::TableSetupColumn("Reg");
        ImGui::TableSetupColumn("Dec");
        ImGui::TableSetupColumn("Hex");
        ImGui::TableHeadersRow();
    
        for (int i = 0; i < NUM_REGS; i++) {
          ImGui::TableNextRow();
          ImGui::TableNextColumn();
          ImGui::Text("%-10s", reg_names[i]);
    
          ImGui::TableNextColumn();
          ImGui::Text("%11d", (int32_t)this->m_reg[i]);
    
          ImGui::TableNextColumn();
          ImGui::Text("0x%08x", this->m_reg[i]);
        }
    
        ImGui::EndTable();
      }
    }
  }
  ImGui::EndChild();
}
