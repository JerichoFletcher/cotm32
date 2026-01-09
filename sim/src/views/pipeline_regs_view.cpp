#include "views/pipeline_regs_view.hpp"

PipelineRegsView::PipelineRegsView(const VerilatedContainer& v) : m_v(v) {}

const IfIdRegView PipelineRegsView::if_id() const {
    IfIdRegView view;
    auto* core = this->m_v.top()->cotm32->core;

    view.valid = core->ifid_valid;
    view.stall = core->ifid_stall;
    view.flush = core->ifid_flush;

    view.pc = core->id_pc;
    view.pc_4 = core->id_pc_4;
    view.inst = core->id_inst;

    return view;
}

const IdExRegView PipelineRegsView::id_ex() const {
    IdExRegView view;
    auto* core = this->m_v.top()->cotm32->core;

    view.valid = core->idex_valid;
    view.stall = core->idex_stall;
    view.flush = core->idex_flush;

    view.alu_op = core->ex_alu_op;
    view.alu_a_sel = core->ex_alu_a_sel;
    view.alu_b_sel = core->ex_alu_b_sel;

    view.bu_be = core->ex_bu_be;
    view.bu_op = core->ex_bu_op;

    view.regfile_we = core->ex_regfile_we;
    view.rd_addr = core->ex_rd_addr;
    view.rs1_addr = core->ex_rs1_addr;
    view.rs2_addr = core->ex_rs2_addr;
    view.lsu_ls_op = core->ex_lsu_ls_op;
    view.reg_wb_sel = core->ex_reg_wb_sel;

    view.csr_we = core->ex_csr_we;
    view.csr_data_sel = core->ex_csr_data_sel;
    view.csr_addr = core->ex_csr_addr;
    view.csr_op = core->ex_csr_op;
    view.csr_zimm = core->ex_csr_zimm;

    view.rs1 = core->ex_rs1;
    view.rs2 = core->ex_rs2;
    view.imm = core->ex_imm;

    view.pc = core->ex_pc;
    view.pc_4 = core->ex_pc_4;

    return view;
}
