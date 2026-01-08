#include "drawers/mem_drawer.hpp"

#include <fmt/core.h>

#include "cotm32_defs.hpp"

static const char* options[3] = {
  "IMEM", "ROM", "DMEM"
};

MemDrawer::MemDrawer(const VerilatedContainer& v):
  m_v(v),
  m_mem_sec_curr(0),
  m_mem_offset(0) {}

void MemDrawer::draw() {
  if (ImGui::BeginChild("mem",
    ImVec2(ImGui::GetContentRegionAvail().x, 0),
    ImGuiChildFlags_AutoResizeY
  )) {
    if (ImGui::CollapsingHeader("Memory Inspector")) {
      ImGui::Combo("Source", &this->m_mem_sec_curr, options, IM_COUNTOF(options));
      
      int window_start;
      int mem_src_size;
      int mem_src_start;
      int mem_src_end;
      int offset_max;
      
      switch (this->m_mem_sec_curr) {
        case 0:
          window_start = INST_MEM_START;
          mem_src_size = INST_MEM_SIZE;
          mem_src_start = INST_MEM_START;
          mem_src_end = INST_MEM_END;
          offset_max = INST_MEM_SIZE - MemDrawer::DISPLAY_WINDOW_W * MemDrawer::DISPLAY_WINDOW_H;
          break;
        case 1:
          window_start = ROM_MEM_START;
          mem_src_size = ROM_MEM_SIZE;
          mem_src_start = ROM_MEM_START;
          mem_src_end = ROM_MEM_END;
          offset_max = ROM_MEM_SIZE - MemDrawer::DISPLAY_WINDOW_W * MemDrawer::DISPLAY_WINDOW_H;
          break;
        case 2: 
          window_start = DATA_MEM_START;
          mem_src_size = DATA_MEM_SIZE;
          mem_src_start = DATA_MEM_START;
          mem_src_end = DATA_MEM_END;
          offset_max = DATA_MEM_SIZE - MemDrawer::DISPLAY_WINDOW_W * MemDrawer::DISPLAY_WINDOW_H;
          break;
      }
      this->m_mem_offset = this->m_mem_offset > offset_max ? offset_max : this->m_mem_offset;

      ImGui::SliderInt("Offset", &this->m_mem_offset, 0, offset_max, "%08x", ImGuiSliderFlags_AlwaysClamp);
      if (ImGui::IsItemDeactivatedAfterEdit()) {
        this->m_mem_offset &= -4;
      }

      ImGui::Separator();
      ImGui::Text("Size     : %d B", mem_src_size);
      ImGui::Text("Address  : %08x:%08x", mem_src_start, mem_src_end);

      if (ImGui::BeginTable("table_mem", MemDrawer::DISPLAY_WINDOW_W + 1,
        ImGuiTableFlags_RowBg
      )) {
        ImGui::TableSetupColumn("ADDR");

        for (int i = 0; i < MemDrawer::DISPLAY_WINDOW_W; i++) {
          ImGui::TableSetupColumn(fmt::format("{:02x}", i).c_str());
        }
        ImGui::TableHeadersRow();

        for (int r = 0; r < MemDrawer::DISPLAY_WINDOW_H; r++) {
          ImGui::TableNextRow();
          ImGui::TableNextColumn();
          ImGui::Text("0x%08x", mem_src_start + this->m_mem_offset + r * MemDrawer::DISPLAY_WINDOW_W);

          for (int c = 0; c < MemDrawer::DISPLAY_WINDOW_W; c++) {
            ImGui::TableNextColumn();
            
            uint32_t addr = mem_src_start + this->m_mem_offset + r * MemDrawer::DISPLAY_WINDOW_W + c;
            uint8_t val;
            if (this->m_v.read_byte(addr, &val)) {
              ImGui::Text("%02x", val);
            } else {
              ImGui::Text("  ");
            }
          }
        }

        ImGui::EndTable();
      }
    }
  }
  ImGui::EndChild();
}
