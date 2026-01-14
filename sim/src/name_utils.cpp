#include "name_utils.hpp"

#include <array>
#include <map>

#include "cotm32_defs.hpp"

template <size_t N>
using StringArray = std::array<std::string, N>;

static const std::string EMPTY;

static const StringArray<NUM_REGS> reg_names = {
    "x0/zero", "x1/ra",  "x2/sp",   "x3/gp",   "x4/tp",  "x5/t0",  "x6/t1",  "x7/t2",
    "x8/s0",   "x9/s1",  "x10/a0",  "x11/a1",  "x12/a2", "x13/a3", "x14/a4", "x15/a5",
    "x16/a6",  "x17/a7", "x18/s2",  "x19/s3",  "x20/s4", "x21/s5", "x22/s6", "x23/s7",
    "x24/s8",  "x25/s9", "x26/s10", "x27/s11", "x28/t3", "x29/t4", "x30/t5", "x31/t6"
};
static const StringArray<10> alu_op_names = {
    "ADD", "SUB", "OR", "AND", "XOR", "SLT", "SLTU", "SLL", "SRL", "SRA"
};
static const StringArray<8> bu_op_names = {"NEVER", "ALWAYS", "EQ", "NE", "LT", "GE", "LTU", "GEU"};

static const StringArray<2> alu_a_names = {"RS1", "PC"};
static const StringArray<2> alu_b_names = {"RS2", "IMM"};
static const StringArray<9> lsu_ls_names = {
    "NONE", "LOAD B", "LOAD H", "LOAD W", "LOAD UB", "LOAD UH", "STORE B", "STORE H", "STORE W"
};
static const StringArray<6> reg_wb_names = {"ZERO", "ALU OUT", "MU OUT", "PC+4", "LSU", "CSR"};

static const StringArray<9> mu_op_names = {
    "NOP", "MUL", "MULH", "MULHU", "MULHSU", "DIV", "DIVU", "REM", "REMU"
};

static const std::map<CsrId, std::string> csr_indices = {
    {CsrId::CsrId_MTVEC,  "mtvec" },
    {CsrId::CsrId_MEPC,   "mepc"  },
    {CsrId::CsrId_MCAUSE, "mcause"},
    {CsrId::CsrId_MTVAL,  "mtval" },
};
static const StringArray<2> csr_write_names = {"RS1", "IMM"};
static const StringArray<4> csr_op_names = {"NONE", "RW", "RS", "RC"};

static const StringArray<3> fwd_src_names = {"NONE", "EX/MEM", "MEM/WB"};

template <typename T, std::size_t size>
inline bool in_bounds(int i, std::array<T, size> arr) {
    return 0 <= i && i < size;
}

namespace cotm32::name_utils {
const std::string& reg_name(uint8_t reg_index) {
    if (!in_bounds(reg_index, reg_names)) return EMPTY;
    return reg_names[reg_index];
}

const std::string& alu_op_name(AluOp alu_op) {
    if (!in_bounds(alu_op, alu_op_names)) return EMPTY;
    return alu_op_names[alu_op];
}

const std::string& bu_op_name(BuOp bu_op) {
    if (!in_bounds(bu_op, bu_op_names)) return EMPTY;
    return bu_op_names[bu_op];
}

const std::string& alu_a_name(AluPortA alu_a_sel) {
    if (!in_bounds(alu_a_sel, alu_a_names)) return EMPTY;
    return alu_a_names[alu_a_sel];
}

const std::string& alu_b_name(AluPortB alu_b_sel) {
    if (!in_bounds(alu_b_sel, alu_b_names)) return EMPTY;
    return alu_b_names[alu_b_sel];
}

const std::string& lsu_ls_op_name(LsuLoadStoreOp lsu_ls_op) {
    if (!in_bounds(lsu_ls_op, lsu_ls_names)) return EMPTY;
    return lsu_ls_names[lsu_ls_op];
}

const std::string& reg_wb_name(RegWritebackSrc reg_wb_sel) {
    if (!in_bounds(reg_wb_sel, reg_wb_names)) return EMPTY;
    return reg_wb_names[reg_wb_sel];
}

const std::string& mu_op_name(MuOp mu_op) {
    if (!in_bounds(mu_op, mu_op_names)) return EMPTY;
    return mu_op_names[mu_op];
}

const std::string& csr_name(CsrId csr_id) {
    auto v = csr_indices.find(csr_id);
    return v != csr_indices.end() ? v->second : EMPTY;
}

const std::string& csr_write_name(CsrWriteSrc csr_write) {
    if (!in_bounds(csr_write, csr_write_names)) return EMPTY;
    return csr_write_names[csr_write];
}

const std::string& csr_op_name(CsrOp csr_op) {
    if (!in_bounds(csr_op, csr_op_names)) return EMPTY;
    return csr_op_names[csr_op];
}

const std::string& pipe_fwd_src_name(FwdSrc fwd_src) {
    if (!in_bounds(fwd_src, fwd_src_names)) return EMPTY;
    return fwd_src_names[fwd_src];
}
}  // namespace cotm32::name_utils
