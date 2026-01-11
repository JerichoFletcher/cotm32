#include "drawers/pipeline_regs_drawer.hpp"

#include "colors.hpp"
#include "cotm32_defs.hpp"
#include "drawers/draw_utils.hpp"
#include "imgui.h"
#include "name_utils.hpp"
#include "views/pipeline_regs_view.hpp"

void draw_sig_vsf(bool valid, bool stall, bool flush) {
    auto c_off = cotm32::colors::OFF;
    auto c_red = cotm32::colors::RED;
    auto c_yellow = cotm32::colors::YELLOW;
    auto c_green = cotm32::colors::GREEN;
    draw_signal("Valid", valid, c_green, c_red, false);
    ImGui::SameLine();
    draw_signal("Stall", stall, c_yellow, c_off, false);
    ImGui::SameLine();
    draw_signal("Flush", flush, c_red, c_off, false);
}

void PipelineRegsDrawer::render(const Simulator& sim) {
    if (ImGui::BeginChild(
            "pipeline_regs",
            ImVec2(ImGui::GetContentRegionAvail().x, 0),
            ImGuiChildFlags_AutoResizeY
        )) {
        if (ImGui::CollapsingHeader("Pipeline Registers", ImGuiTreeNodeFlags_DefaultOpen)) {
            PipelineRegsView view(sim.v());

            if (ImGui::CollapsingHeader("IF/ID", ImGuiTreeNodeFlags_DefaultOpen)) {
                auto phase = view.if_id();
                draw_sig_vsf(phase.valid, phase.stall, phase.flush);

                ImGui::Text("PC          : 0x%08x", phase.pc);
                ImGui::Text("PC+4        : 0x%08x", phase.pc_4);
                ImGui::Text("Instruction : 0x%08x", phase.inst);
            }

            if (ImGui::CollapsingHeader("ID/EX", ImGuiTreeNodeFlags_DefaultOpen)) {
                auto phase = view.id_ex();
                draw_sig_vsf(phase.valid, phase.stall, phase.flush);

                ImGui::Text("PC          : 0x%08x", phase.pc);
                ImGui::Text("PC+4        : 0x%08x", phase.pc_4);
                ImGui::Text("Instruction : 0x%08x", phase.inst);

                ImGui::Separator();
                auto& alu_op_name = cotm32::name_utils::alu_op_name(phase.alu_op);
                if (!alu_op_name.empty()) {
                    ImGui::Text("ALU Op      : %s", alu_op_name.c_str());
                } else {
                    ImGui::Text("ALU Op      : ??? (%d)", phase.alu_op);
                }

                auto& alu_a_name = cotm32::name_utils::alu_a_name(phase.alu_a_sel);
                if (!alu_a_name.empty()) {
                    ImGui::Text("ALU Port A  : %s", alu_a_name.c_str());
                } else {
                    ImGui::Text("ALU Port A  : ??? (%d)", phase.alu_a_sel);
                }

                auto& alu_b_name = cotm32::name_utils::alu_b_name(phase.alu_b_sel);
                if (!alu_b_name.empty()) {
                    ImGui::Text("ALU Port B  : %s", alu_b_name.c_str());
                } else {
                    ImGui::Text("ALU Port B  : ??? (%d)", phase.alu_b_sel);
                }

                ImGui::Separator();
                draw_signal(
                    "Branch Enable", phase.bu_be, cotm32::colors::GREEN, cotm32::colors::OFF, false
                );
                auto& bu_op = cotm32::name_utils::bu_op_name(phase.bu_op);
                if (!bu_op.empty()) {
                    ImGui::Text("BU Op       : %s", bu_op.c_str());
                } else {
                    ImGui::Text("Bu Op       : ??? (%d)", phase.bu_op);
                }

                ImGui::Separator();
                draw_signal(
                    "CSR Write Enable",
                    phase.csr_we,
                    cotm32::colors::GREEN,
                    cotm32::colors::OFF,
                    false
                );
                auto& csr_name = cotm32::name_utils::csr_name(phase.csr_addr);
                if (!csr_name.empty()) {
                    ImGui::Text("CSR         : %s", csr_name.c_str());
                } else {
                    ImGui::Text("CSR         : ??? (%d)", phase.csr_addr);
                }
                auto& csr_op_name = cotm32::name_utils::csr_op_name(phase.csr_op);
                if (!csr_op_name.empty()) {
                    ImGui::Text("CSR Op      : %s", csr_op_name.c_str());
                } else {
                    ImGui::Text("CSR Op      : ??? (%d)", phase.csr_op);
                }
                auto& csr_write_name = cotm32::name_utils::csr_write_name(phase.csr_data_sel);
                if (!csr_write_name.empty()) {
                    ImGui::Text("CSR Data    : %s", csr_write_name.c_str());
                } else {
                    ImGui::Text("CSR Data    : ??? (%d)", phase.csr_data_sel);
                }

                ImGui::Separator();
                ImGui::Text(
                    "RS1 Address : %s", cotm32::name_utils::reg_name(phase.rs1_addr).c_str()
                );
                ImGui::Text("RS1         : 0x%08x (%d)", phase.rs1, phase.rs1);
                ImGui::Text(
                    "RS2 Address : %s", cotm32::name_utils::reg_name(phase.rs2_addr).c_str()
                );
                ImGui::Text("RS2         : 0x%08x (%d)", phase.rs2, phase.rs2);
                ImGui::Text("Immediate   : 0x%08x (%d)", phase.imm, phase.imm);

                ImGui::Separator();
                ImGui::TextUnformatted("RD Address  :");
                ImGui::SameLine();
                draw_signal(
                    cotm32::name_utils::reg_name(phase.rd_addr),
                    phase.regfile_we,
                    cotm32::colors::GREEN,
                    cotm32::colors::OFF,
                    false
                );
                auto& reg_wb_name = cotm32::name_utils::reg_wb_name(phase.reg_wb_sel);
                if (!reg_wb_name.empty()) {
                    ImGui::Text("Register WB : %s", reg_wb_name.c_str());
                } else {
                    ImGui::Text("Register WB : ??? (%d)", phase.reg_wb_sel);
                }
                auto& lsu_ls_name = cotm32::name_utils::lsu_ls_op_name(phase.lsu_ls_op);
                if (!lsu_ls_name.empty()) {
                    ImGui::Text("LSU L/S Op  : %s", lsu_ls_name.c_str());
                } else {
                    ImGui::Text("LSU L/S Op  : ??? (%d)", phase.lsu_ls_op);
                }
            }

            if (ImGui::CollapsingHeader("EX/MEM", ImGuiTreeNodeFlags_DefaultOpen)) {
                auto phase = view.ex_mem();
                draw_sig_vsf(phase.valid, phase.stall, phase.flush);

                ImGui::Text("PC          : 0x%08x", phase.pc);
                ImGui::Text("PC+4        : 0x%08x", phase.pc_4);
                ImGui::Text("Instruction : 0x%08x", phase.inst);

                ImGui::Separator();
                ImGui::Text("ALU Out     : 0x%08x (%d)", phase.alu_out, phase.alu_out);
                ImGui::Text("RS1         : 0x%08x (%d)", phase.rs1, phase.rs1);
                ImGui::Text("RS2         : 0x%08x (%d)", phase.rs2, phase.rs2);

                ImGui::Separator();
                draw_signal(
                    "CSR Write Enable",
                    phase.csr_we,
                    cotm32::colors::GREEN,
                    cotm32::colors::OFF,
                    false
                );
                auto& csr_name = cotm32::name_utils::csr_name(phase.csr_addr);
                if (!csr_name.empty()) {
                    ImGui::Text("CSR         : %s", csr_name.c_str());
                } else {
                    ImGui::Text("CSR         : ??? (%d)", phase.csr_addr);
                }
                auto& csr_op_name = cotm32::name_utils::csr_op_name(phase.csr_op);
                if (!csr_op_name.empty()) {
                    ImGui::Text("CSR Op      : %s", csr_op_name.c_str());
                } else {
                    ImGui::Text("CSR Op      : ??? (%d)", phase.csr_op);
                }
                auto& csr_write_name = cotm32::name_utils::csr_write_name(phase.csr_data_sel);
                if (!csr_write_name.empty()) {
                    ImGui::Text("CSR Data    : %s", csr_write_name.c_str());
                } else {
                    ImGui::Text("CSR Data    : ??? (%d)", phase.csr_data_sel);
                }

                ImGui::Separator();
                ImGui::TextUnformatted("RD Address  :");
                ImGui::SameLine();
                draw_signal(
                    cotm32::name_utils::reg_name(phase.rd_addr),
                    phase.regfile_we,
                    cotm32::colors::GREEN,
                    cotm32::colors::OFF,
                    false
                );
                auto& reg_wb_name = cotm32::name_utils::reg_wb_name(phase.reg_wb_sel);
                if (!reg_wb_name.empty()) {
                    ImGui::Text("Register WB : %s", reg_wb_name.c_str());
                } else {
                    ImGui::Text("Register WB : ??? (%d)", phase.reg_wb_sel);
                }
                auto& lsu_ls_name = cotm32::name_utils::lsu_ls_op_name(phase.lsu_ls_op);
                if (!lsu_ls_name.empty()) {
                    ImGui::Text("LSU L/S Op  : %s", lsu_ls_name.c_str());
                } else {
                    ImGui::Text("LSU L/S Op  : ??? (%d)", phase.lsu_ls_op);
                }
            }

            if (ImGui::CollapsingHeader("MEM/WB", ImGuiTreeNodeFlags_DefaultOpen)) {
                auto phase = view.mem_wb();
                draw_sig_vsf(phase.valid, phase.stall, phase.flush);

                ImGui::Text("PC          : 0x%08x", phase.pc);
                ImGui::Text("PC+4        : 0x%08x", phase.pc_4);
                ImGui::Text("Instruction : 0x%08x", phase.inst);

                ImGui::Separator();
                ImGui::Text("ALU Out     : 0x%08x (%d)", phase.alu_out, phase.alu_out);
                ImGui::Text("LSU Read    : 0x%08x (%d)", phase.lsu_rdata, phase.lsu_rdata);
                ImGui::Text("CSR Read    : 0x%08x (%d)", phase.csr_rdata, phase.csr_rdata);

                ImGui::Separator();
                ImGui::TextUnformatted("RD Address  :");
                ImGui::SameLine();
                draw_signal(
                    cotm32::name_utils::reg_name(phase.rd_addr),
                    phase.regfile_we,
                    cotm32::colors::GREEN,
                    cotm32::colors::OFF,
                    false
                );
                auto& reg_wb_name = cotm32::name_utils::reg_wb_name(phase.reg_wb_sel);
                if (!reg_wb_name.empty()) {
                    ImGui::Text("Register WB : %s", reg_wb_name.c_str());
                } else {
                    ImGui::Text("Register WB : ??? (%d)", phase.reg_wb_sel);
                }
            }
        }
    }
    ImGui::EndChild();
}
