#pragma once

#include "cotm32_defs.hpp"
#include "verilated_container.hpp"

struct IfIdRegView {
    bool valid;
    bool stall;
    bool flush;

    IData pc;
    IData pc_4;
    IData inst;
};

struct IdExRegView {
    bool valid;
    bool stall;
    bool flush;

    AluOp alu_op;
    AluPortA alu_a_sel;
    AluPortB alu_b_sel;

    bool bu_be;
    BuOp bu_op;

    bool regfile_we;
    CData rd_addr;
    CData rs1_addr;
    CData rs2_addr;
    LsuLoadStoreOp lsu_ls_op;
    RegWritebackSrc reg_wb_sel;

    bool csr_we;
    CsrWriteSrc csr_data_sel;
    CsrId csr_addr;
    CsrOp csr_op;
    IData csr_zimm;

    IData rs1;
    IData rs2;
    IData imm;

    IData pc;
    IData pc_4;
};

struct ExMemRegView {
    bool valid;
    bool stall;
    bool flush;

    IData alu_out;
    IData rs2;

    bool regfile_we;
    CData rd_addr;
    LsuLoadStoreOp lsu_ls_op;
    RegWritebackSrc reg_wb_sel;

    bool csr_we;
    CsrWriteSrc csr_data_sel;
    CsrId csr_addr;
    CsrOp csr_op;
    IData csr_zimm;

    IData pc;
    IData pc_4;
};

struct MemWbRegView {
    bool valid;
    bool stall;
    bool flush;

    IData lsu_rdata;
    IData alu_out;
    IData csr_rdata;

    bool regfile_we;
    CData rd_addr;
    RegWritebackSrc reg_wb_sel;

    IData pc;
    IData pc_4;
};

class PipelineRegsView {
public:
    PipelineRegsView(const VerilatedContainer& v);

    const IfIdRegView if_id() const;
    const IdExRegView id_ex() const;
    const ExMemRegView ex_mem() const;
    const MemWbRegView mem_wb() const;

private:
    const VerilatedContainer& m_v;
};
