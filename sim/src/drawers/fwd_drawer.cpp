#include "drawers/fwd_drawer.hpp"

#include "colors.hpp"
#include "drawers/draw_utils.hpp"
#include "name_utils.hpp"
#include "views/fwd_view.hpp"

using cotm32::draw_utils::draw_signal;

void FwdDrawer::render(const Simulator& sim) {
    if (ImGui::BeginChild(
            "fwd", ImVec2(ImGui::GetContentRegionAvail().x, 0), ImGuiChildFlags_AutoResizeY
        )) {
        if (ImGui::CollapsingHeader("Forwarding Unit", ImGuiTreeNodeFlags_DefaultOpen)) {
            FwdView view(sim.v());

            ImGui::SeparatorText("Forward Sources");
            auto& mem_rd_addr_name = cotm32::name_utils::reg_name(view.mem_rd_addr());
            if (!mem_rd_addr_name.empty()) {
                ImGui::Text("MEM RD Dest : %s", mem_rd_addr_name.c_str());
            } else {
                ImGui::Text("MEM RD Dest : ??? (%d)", view.mem_rd_addr());
            }
            auto& wb_rd_addr_name = cotm32::name_utils::reg_name(view.wb_rd_addr());
            if (!wb_rd_addr_name.empty()) {
                ImGui::Text("WB RD Dest  : %s", wb_rd_addr_name.c_str());
            } else {
                ImGui::Text("WB RD Dest  : ??? (%d)", view.wb_rd_addr());
            }

            ImGui::SeparatorText("RS1");
            auto& rs1_addr_name = cotm32::name_utils::reg_name(view.ex_rs1_addr());
            if (!rs1_addr_name.empty()) {
                ImGui::Text("RS1 Source  : %s", rs1_addr_name.c_str());
            } else {
                ImGui::Text("RS1 Source  : ??? (%d)", view.ex_rs1_addr());
            }
            ImGui::Text("RS1 Value   : 0x%08x (%d)", view.ex_rs1_fwd(), view.ex_rs1_fwd());

            ImGui::TextUnformatted("Forward     :");
            ImGui::SameLine();
            auto& rs1_fwd_name = cotm32::name_utils::pipe_fwd_src_name(view.forward_a_src());
            draw_signal(
                !rs1_fwd_name.empty() ? rs1_fwd_name.c_str() : "???",
                view.forward_a_src() != FwdSrc_NONE,
                cotm32::colors::GREEN,
                cotm32::colors::OFF,
                false
            );

            ImGui::SeparatorText("RS2");
            auto& rs2_addr_name = cotm32::name_utils::reg_name(view.ex_rs2_addr());
            if (!rs2_addr_name.empty()) {
                ImGui::Text("RS2 Source  : %s", rs2_addr_name.c_str());
            } else {
                ImGui::Text("RS2 Source  : ??? (%d)", view.ex_rs2_addr());
            }
            ImGui::Text("RS2 Value   : 0x%08x (%d)", view.ex_rs2_fwd(), view.ex_rs2_fwd());

            ImGui::TextUnformatted("Forward     :");
            ImGui::SameLine();
            auto& rs2_fwd_name = cotm32::name_utils::pipe_fwd_src_name(view.forward_b_src());
            draw_signal(
                !rs2_fwd_name.empty() ? rs2_fwd_name.c_str() : "???",
                view.forward_b_src() != FwdSrc_NONE,
                cotm32::colors::GREEN,
                cotm32::colors::OFF,
                false
            );
        }
    }
    ImGui::EndChild();
}
