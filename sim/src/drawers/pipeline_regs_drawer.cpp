#include "drawers/pipeline_regs_drawer.hpp"

#include "cotm32_defs.hpp"
#include "drawers/draw_utils.hpp"
#include "imgui.h"
#include "views/pipeline_regs_view.hpp"

static const char* reg_names[NUM_REGS] = {
    "x0/zero", "x1/ra",  "x2/sp",   "x3/gp",   "x4/tp",  "x5/t0",  "x6/t1",  "x7/t2",
    "x8/s0",   "x9/s1",  "x10/a0",  "x11/a1",  "x12/a2", "x13/a3", "x14/a4", "x15/a5",
    "x16/a6",  "x17/a7", "x18/s2",  "x19/s3",  "x20/s4", "x21/s5", "x22/s6", "x23/s7",
    "x24/s8",  "x25/s9", "x26/s10", "x27/s11", "x28/t3", "x29/t4", "x30/t5", "x31/t6"
};

static const char* alu_op_names[10] = {
    "ADD", "SUB", "OR", "AND", "XOR", "SLT", "SLTU", "SLL", "SRL", "SRA"
};
static const char* bu_op_names[6] = {"EQ", "NE", "LT", "GE", "LTU", "GEU"};
static const char* alu_a_names[2] = {"RS1", "PC"};
static const char* alu_b_names[2] = {"RS2", "IMM"};
static const char* lsu_ls_names[9] = {
    "NONE", "LOAD B", "LOAD H", "LOAD W", "LOAD UB", "LOAD UH", "STORE B", "STORE H", "STORE W"
};
static const char* reg_wb_names[5] = {"ZERO", "ALU OUT", "PC+4", "LSU", "CSR"};

void draw_sig_vsf(bool valid, bool stall, bool flush) {
    draw_signal("Valid", valid, IM_COL32(40, 255, 40, 255), IM_COL32(255, 40, 40, 255), false);
    ImGui::SameLine();
    draw_signal("Stall", stall, IM_COL32(255, 255, 40, 255), IM_COL32(40, 40, 40, 255), false);
    ImGui::SameLine();
    draw_signal("Flush", flush, IM_COL32(255, 40, 40, 255), IM_COL32(40, 40, 40, 255), false);
}

void PipelineRegsDrawer::render(const Simulator& sim) {
    if (ImGui::BeginChild(
            "pipeline_regs",
            ImVec2(ImGui::GetContentRegionAvail().x, 0),
            ImGuiChildFlags_AutoResizeY
        )) {
        if (ImGui::CollapsingHeader("Pipeline Registers", ImGuiTreeNodeFlags_DefaultOpen)) {
            PipelineRegsView view(sim.v());

            if (ImGui::CollapsingHeader("IF/ID")) {
                auto ifid = view.if_id();
                draw_sig_vsf(ifid.valid, ifid.stall, ifid.flush);

                ImGui::Text("PC          : 0x%08x", ifid.pc);
                ImGui::Text("PC+4        : 0x%08x", ifid.pc_4);

                ImGui::Separator();
                ImGui::Text("Instruction : 0x%08x", ifid.inst);
            }

            if (ImGui::CollapsingHeader("ID/EX")) {
                auto idex = view.id_ex();
                draw_sig_vsf(idex.valid, idex.stall, idex.flush);

                ImGui::Text("PC          : 0x%08x", idex.pc);
                ImGui::Text("PC+4        : 0x%08x", idex.pc_4);

                ImGui::Separator();
                if (0 <= idex.alu_op && idex.alu_op < IM_COUNTOF(alu_op_names)) {
                    ImGui::Text("ALU Op      : %s", alu_op_names[idex.alu_op]);
                } else {
                    ImGui::Text("ALU Op      : ??? (%d)", idex.alu_op);
                }

                if (0 <= idex.alu_a_sel && idex.alu_a_sel < IM_COUNTOF(alu_a_names)) {
                    ImGui::Text("ALU Port A  : %s", alu_a_names[idex.alu_a_sel]);
                } else {
                    ImGui::Text("ALU Port A  : ??? (%d)", idex.alu_a_sel);
                }

                if (0 <= idex.alu_b_sel && idex.alu_b_sel < IM_COUNTOF(alu_b_names)) {
                    ImGui::Text("ALU Port B  : %s", alu_b_names[idex.alu_b_sel]);
                } else {
                    ImGui::Text("ALU Port B  : ??? (%d)", idex.alu_b_sel);
                }

                ImGui::Separator();
                draw_signal(
                    "Branch Enable",
                    idex.bu_be,
                    IM_COL32(40, 255, 40, 255),
                    IM_COL32(40, 40, 40, 255),
                    false
                );
                if (0 <= idex.bu_op && idex.bu_op < IM_COUNTOF(bu_op_names)) {
                    ImGui::Text("Register WB : %s", bu_op_names[idex.bu_op]);
                } else {
                    ImGui::Text("Register WB : ??? (%d)", idex.bu_op);
                }

                ImGui::Separator();
                ImGui::Text("RS1 Address : %s", reg_names[idex.rs1_addr]);
                ImGui::Text("  -> (Dec)  : %d", idex.rs1);
                ImGui::Text("  -> (Hex)  : 0x%08x", idex.rs1);
                ImGui::Text("RS2 Address : %s", reg_names[idex.rs2_addr]);
                ImGui::Text("  -> (Dec)  : %d", idex.rs2);
                ImGui::Text("  -> (Hex)  : 0x%08x", idex.rs2);
                ImGui::TextUnformatted("Immediate");
                ImGui::Text("  -> (Dec)  : %d", idex.imm);
                ImGui::Text("  -> (Hex)  : 0x%08x", idex.imm);

                ImGui::Separator();
                ImGui::TextUnformatted("RD Address  :");
                ImGui::SameLine();
                draw_signal(
                    reg_names[idex.rd_addr],
                    idex.regfile_we,
                    IM_COL32(40, 255, 40, 255),
                    IM_COL32(40, 40, 40, 255),
                    false
                );
                if (0 <= idex.reg_wb_sel && idex.reg_wb_sel < IM_COUNTOF(reg_wb_names)) {
                    ImGui::Text("Register WB : %s", reg_wb_names[idex.reg_wb_sel]);
                } else {
                    ImGui::Text("Register WB : ??? (%d)", idex.reg_wb_sel);
                }
                if (0 <= idex.lsu_ls_op && idex.lsu_ls_op < IM_COUNTOF(lsu_ls_names)) {
                    ImGui::Text("LSU L/S Op  : %s", lsu_ls_names[idex.lsu_ls_op]);
                } else {
                    ImGui::Text("LSU L/S Op  : ??? (%d)", idex.lsu_ls_op);
                }
            }

            if (ImGui::CollapsingHeader("EX/MEM")) {
                auto exmem = view.ex_mem();
                draw_sig_vsf(exmem.valid, exmem.stall, exmem.flush);

                ImGui::Text("PC          : 0x%08x", exmem.pc);
                ImGui::Text("PC+4        : 0x%08x", exmem.pc_4);

                ImGui::Separator();
                ImGui::TextUnformatted("ALU Out");
                ImGui::Text("  -> (Dec)  : %d", exmem.alu_out);
                ImGui::Text("  -> (Hex)  : 0x%08x", exmem.alu_out);
                ImGui::TextUnformatted("RS2");
                ImGui::Text("  -> (Dec)  : %d", exmem.rs2);
                ImGui::Text("  -> (Hex)  : 0x%08x", exmem.rs2);

                ImGui::Separator();
                ImGui::TextUnformatted("RD Address  :");
                ImGui::SameLine();
                draw_signal(
                    reg_names[exmem.rd_addr],
                    exmem.regfile_we,
                    IM_COL32(40, 255, 40, 255),
                    IM_COL32(40, 40, 40, 255),
                    false
                );
                if (0 <= exmem.reg_wb_sel && exmem.reg_wb_sel < IM_COUNTOF(reg_wb_names)) {
                    ImGui::Text("Register WB : %s", reg_wb_names[exmem.reg_wb_sel]);
                } else {
                    ImGui::Text("Register WB : ??? (%d)", exmem.reg_wb_sel);
                }
                if (0 <= exmem.lsu_ls_op && exmem.lsu_ls_op < IM_COUNTOF(lsu_ls_names)) {
                    ImGui::Text("LSU L/S Op  : %s", lsu_ls_names[exmem.lsu_ls_op]);
                } else {
                    ImGui::Text("LSU L/S Op  : ??? (%d)", exmem.lsu_ls_op);
                }
            }
        }
    }
    ImGui::EndChild();
}
