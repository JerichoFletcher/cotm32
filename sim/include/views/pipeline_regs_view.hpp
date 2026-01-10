#pragma once

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

    CData alu_op;
    CData alu_a_sel;
    CData alu_b_sel;

    bool bu_be;
    CData bu_op;

    bool regfile_we;
    CData rd_addr;
    CData rs1_addr;
    CData rs2_addr;
    CData lsu_ls_op;
    CData reg_wb_sel;

    bool csr_we;
    CData csr_data_sel;
    SData csr_addr;
    CData csr_op;
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
    CData lsu_ls_op;
    CData reg_wb_sel;

    bool csr_we;
    CData csr_data_sel;
    SData csr_addr;
    CData csr_op;
    IData csr_zimm;

    IData pc;
    IData pc_4;
};

class PipelineRegsView {
public:
    PipelineRegsView(const VerilatedContainer& v);

    const IfIdRegView if_id() const;
    const IdExRegView id_ex() const;
    const ExMemRegView ex_mem() const;

private:
    const VerilatedContainer& m_v;
};
