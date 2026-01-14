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

    view.alu_op = static_cast<AluOp>(core->ex_alu_op);
    view.alu_a_sel = static_cast<AluPortA>(core->ex_alu_a_sel);
    view.alu_b_sel = static_cast<AluPortB>(core->ex_alu_b_sel);

    view.bu_be = core->ex_bu_be;
    view.bu_op = static_cast<BuOp>(core->ex_bu_op);

    view.regfile_we = core->ex_regfile_we;
    view.rd_addr = core->ex_rd_addr;
    view.rs1_addr = core->ex_rs1_addr;
    view.rs2_addr = core->ex_rs2_addr;
    view.lsu_ls_op = static_cast<LsuLoadStoreOp>(core->ex_lsu_ls_op);
    view.reg_wb_sel = static_cast<RegWritebackSrc>(core->ex_reg_wb_sel);

    view.mu_op = static_cast<MuOp>(core->ex_mu_op);

    view.csr_we = core->ex_csr_we;
    view.csr_data_sel = static_cast<CsrWriteSrc>(core->ex_csr_data_sel);
    view.csr_addr = static_cast<CsrId>(core->ex_csr_addr);
    view.csr_op = static_cast<CsrOp>(core->ex_csr_op);
    view.csr_zimm = core->ex_csr_zimm;

    view.rs1 = core->ex_rs1;
    view.rs2 = core->ex_rs2;
    view.imm = core->ex_imm;

    view.pc = core->ex_pc;
    view.pc_4 = core->ex_pc_4;
    view.inst = core->ex_inst;

    return view;
}

const ExMemRegView PipelineRegsView::ex_mem() const {
    ExMemRegView view;
    auto* core = this->m_v.top()->cotm32->core;

    view.valid = core->exmem_valid;
    view.stall = core->exmem_stall;
    view.flush = core->exmem_flush;

    view.alu_out = core->mem_alu_out;
    view.mu_out = core->mem_mu_out;
    view.rs1 = core->mem_rs1;
    view.rs2 = core->mem_rs2;

    view.regfile_we = core->mem_regfile_we;
    view.rd_addr = core->mem_rd_addr;
    view.lsu_ls_op = static_cast<LsuLoadStoreOp>(core->mem_lsu_ls_op);
    view.reg_wb_sel = static_cast<RegWritebackSrc>(core->mem_reg_wb_sel);

    view.csr_we = core->mem_csr_we;
    view.csr_data_sel = static_cast<CsrWriteSrc>(core->mem_csr_data_sel);
    view.csr_addr = static_cast<CsrId>(core->mem_csr_addr);
    view.csr_op = static_cast<CsrOp>(core->mem_csr_op);
    view.csr_zimm = core->mem_csr_zimm;

    view.pc = core->mem_pc;
    view.pc_4 = core->mem_pc_4;
    view.inst = core->mem_inst;

    return view;
}

const MemWbRegView PipelineRegsView::mem_wb() const {
    MemWbRegView view;
    auto* core = this->m_v.top()->cotm32->core;

    view.valid = core->memwb_valid;
    view.stall = core->memwb_stall;
    view.flush = core->memwb_flush;

    view.lsu_rdata = core->wb_lsu_rdata;
    view.alu_out = core->wb_alu_out;
    view.mu_out = core->wb_mu_out;
    view.csr_rdata = core->wb_csr_rdata;

    view.regfile_we = core->wb_regfile_we;
    view.rd_addr = core->wb_rd_addr;
    view.reg_wb_sel = static_cast<RegWritebackSrc>(core->wb_reg_wb_sel);

    view.pc = core->wb_pc;
    view.pc_4 = core->wb_pc_4;
    view.inst = core->wb_inst;

    return view;
}
