#include "drawers/mem_drawer.hpp"

#include <fmt/core.h>

#include "cotm32_defs.hpp"

static const char* mem_src_options[3] = {
  "IMEM", "ROM", "DMEM"
};
static const char* mem_disp_w_options[3] = {
  "Byte", "Half (2 bytes)", "Word (4 bytes)"
};

MemDrawer::MemDrawer(const VerilatedContainer& v):
  m_v(v),
  m_mem_sec_curr(0),
  m_mem_offset(0),
  m_mem_disp_w(0) {}

void MemDrawer::draw() {
  if (ImGui::BeginChild("mem",
    ImVec2(ImGui::GetContentRegionAvail().x, 0),
    ImGuiChildFlags_AutoResizeY
  )) {
    if (ImGui::CollapsingHeader("Memory Inspector", ImGuiTreeNodeFlags_DefaultOpen)) {
      ImGui::Combo("Source", &this->m_mem_sec_curr, mem_src_options, IM_COUNTOF(mem_src_options));
      
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

      if (ImGui::SliderInt("Offset", &this->m_mem_offset, 0, offset_max, "%08x",
        ImGuiSliderFlags_AlwaysClamp
      )) {
        this->m_mem_offset &= -MemDrawer::DISPLAY_WINDOW_W;
      }

      ImGui::Separator();
      ImGui::Text("Size     : %d B", mem_src_size);
      ImGui::Text("Address  : %08x:%08x", mem_src_start, mem_src_end);

      ImGui::Combo("Width", &this->m_mem_disp_w, mem_disp_w_options, IM_COUNTOF(mem_disp_w_options));
      int disp_w;
      switch (this->m_mem_disp_w) {
        case 0: disp_w = 1; break;
        case 1: disp_w = 2; break;
        case 2: disp_w = 4; break;
      }

      if (ImGui::BeginTable("table_mem", MemDrawer::DISPLAY_WINDOW_W / disp_w + 1,
        ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_RowBg
      )) {
        ImGui::TableSetupColumn("ADDR");

        for (int i = 0; i < MemDrawer::DISPLAY_WINDOW_W; i += disp_w) {
          ImGui::TableSetupColumn(fmt::format("{:02x}", i).c_str(), ImGuiTableColumnFlags_WidthStretch);
        }
        ImGui::TableHeadersRow();

        for (int r = 0; r < MemDrawer::DISPLAY_WINDOW_H; r++) {
          ImGui::TableNextRow();
          ImGui::TableNextColumn();
          ImGui::Text("0x%08x", mem_src_start + this->m_mem_offset + r * MemDrawer::DISPLAY_WINDOW_W);

          for (int c = 0; c < MemDrawer::DISPLAY_WINDOW_W; c += disp_w) {
            ImGui::TableNextColumn();
            
            uint32_t addr_base = mem_src_start + this->m_mem_offset + r * MemDrawer::DISPLAY_WINDOW_W + c;
            uint8_t val8_0, val8_1, val8_2, val8_3;
            switch (this->m_mem_disp_w) {
              case 0:
                if (this->m_v.read_byte(addr_base, &val8_0)) {
                  ImGui::Text("%02x", val8_0);
                }
                break;
              case 1:
                if (
                  this->m_v.read_byte(addr_base, &val8_0) &&
                  this->m_v.read_byte(addr_base + 1, &val8_1)
                ) {
                  uint16_t val16 = (((uint16_t)val8_1 & 0xff) << 8) | ((uint16_t)val8_0 & 0xff);
                  ImGui::Text("%04x", val16);
                }
                break;
              case 2:
                if (
                  this->m_v.read_byte(addr_base, &val8_0) &&
                  this->m_v.read_byte(addr_base + 1, &val8_1) &&
                  this->m_v.read_byte(addr_base + 2, &val8_2) &&
                  this->m_v.read_byte(addr_base + 3, &val8_3)
                ) {
                  uint32_t val32 =
                    (((uint16_t)val8_3 & 0xff) << 24) |
                    (((uint16_t)val8_2 & 0xff) << 16) |
                    (((uint16_t)val8_1 & 0xff) << 8) |
                    ((uint16_t)val8_0 & 0xff);
                  ImGui::Text("%08x", val32);
                }
                break;
            }
          }
        }

        ImGui::EndTable();
      }
    }
  }
  ImGui::EndChild();
}
