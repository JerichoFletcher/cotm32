#include "drawers/mem_drawer.hpp"

#include <fmt/core.h>

#include "colors.hpp"
#include "cotm32_defs.hpp"
#include "drawers/draw_utils.hpp"
#include "imgui.h"
#include "views/mem_view.hpp"

using cotm32::draw_utils::draw_signal;

static const char* mem_src_options[2] = {"IMEM", "DMEM"};
static const char* mem_disp_w_options[4] = {"Byte", "Half (2 bytes)", "Word (4 bytes)", "ASCII"};

MemDrawer::MemDrawer() : m_mem_sec_curr(0), m_mem_offset(0), m_mem_disp_w(0) {}

void MemDrawer::render(const Simulator& sim) {
    if (ImGui::BeginChild(
            "mem", ImVec2(ImGui::GetContentRegionAvail().x, 0), ImGuiChildFlags_AutoResizeY
        )) {
        if (ImGui::CollapsingHeader("Memory Inspector", ImGuiTreeNodeFlags_DefaultOpen)) {
            MemView view(sim.v());
            draw_signal(
                "Write Enable",
                view.write_enable(),
                cotm32::colors::GREEN,
                cotm32::colors::OFF,
                false
            );

            ImGui::Separator();
            ImGui::Combo(
                "Source", &this->m_mem_sec_curr, mem_src_options, IM_COUNTOF(mem_src_options)
            );

            int mem_src_size;
            int mem_src_start;
            int mem_src_end;
            int offset_max;

            switch (this->m_mem_sec_curr) {
                case 0:
                    mem_src_size = IMEM_MEM_SIZE;
                    mem_src_start = IMEM_MEM_START;
                    mem_src_end = IMEM_MEM_END;
                    offset_max = IMEM_MEM_SIZE - DISPLAY_WINDOW_W * DISPLAY_WINDOW_H;
                    break;
                case 1:
                    mem_src_size = DATA_MEM_SIZE;
                    mem_src_start = DATA_MEM_START;
                    mem_src_end = DATA_MEM_END;
                    offset_max = DATA_MEM_SIZE - DISPLAY_WINDOW_W * DISPLAY_WINDOW_H;
                    break;
            }
            this->m_mem_offset = this->m_mem_offset > offset_max ? offset_max : this->m_mem_offset;

            ImGui::Text("Size     : %d B", mem_src_size);
            ImGui::Text("Address  : %08x:%08x", mem_src_start, mem_src_end);

            ImGui::Combo(
                "Width", &this->m_mem_disp_w, mem_disp_w_options, IM_COUNTOF(mem_disp_w_options)
            );
            int disp_w;
            switch (this->m_mem_disp_w) {
                case 0: disp_w = 1; break;
                case 1: disp_w = 2; break;
                case 2: disp_w = 4; break;
                case 3: disp_w = 1; break;
            }

            if (ImGui::SliderInt(
                    "Offset",
                    &this->m_mem_offset,
                    0,
                    offset_max,
                    "%08x",
                    ImGuiSliderFlags_AlwaysClamp
                )) {
                this->m_mem_offset &= -DISPLAY_WINDOW_W;
            }

            if (ImGui::BeginTable(
                    "table_mem",
                    DISPLAY_WINDOW_W / disp_w + 1,
                    ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_RowBg
                )) {
                ImGui::TableSetupColumn("ADDR");

                for (int i = 0; i < DISPLAY_WINDOW_W; i += disp_w) {
                    ImGui::TableSetupColumn(
                        fmt::format("{:02x}", i).c_str(), ImGuiTableColumnFlags_WidthStretch
                    );
                }
                ImGui::TableHeadersRow();

                for (int r = 0; r < DISPLAY_WINDOW_H; r++) {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text(
                        "0x%08x", mem_src_start + this->m_mem_offset + r * DISPLAY_WINDOW_W
                    );

                    for (int c = 0; c < DISPLAY_WINDOW_W; c += disp_w) {
                        ImGui::TableNextColumn();

                        uint32_t addr =
                            mem_src_start + this->m_mem_offset + r * DISPLAY_WINDOW_W + c;

                        uint8_t val8;
                        uint16_t val16;
                        uint32_t val32;
                        switch (this->m_mem_disp_w) {
                            case 0:
                                if (view.read_byte(addr, val8)) {
                                    ImGui::Text("%02x", val8);
                                }
                                break;
                            case 1:
                                if (view.read_half(addr, val16)) {
                                    ImGui::Text("%04x", val16);
                                }
                                break;
                            case 2:
                                if (view.read_word(addr, val32)) {
                                    ImGui::Text("%08x", val32);
                                }
                                break;
                            case 3:
                                if (view.read_byte(addr, val8)) {
                                    if (0 < val8 && val8 < 0x10000) {
                                        ImGui::Text("%c", val8);
                                    } else {
                                        ImGui::TextUnformatted(".");
                                    }
                                    break;
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
