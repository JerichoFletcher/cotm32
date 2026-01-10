#include "name_utils.hpp"

#include <array>
#include <map>

#include "cotm32_defs.hpp"

static const std::string EMPTY;

static const std::array<std::string, NUM_REGS> reg_names = {
    "x0/zero", "x1/ra",  "x2/sp",   "x3/gp",   "x4/tp",  "x5/t0",  "x6/t1",  "x7/t2",
    "x8/s0",   "x9/s1",  "x10/a0",  "x11/a1",  "x12/a2", "x13/a3", "x14/a4", "x15/a5",
    "x16/a6",  "x17/a7", "x18/s2",  "x19/s3",  "x20/s4", "x21/s5", "x22/s6", "x23/s7",
    "x24/s8",  "x25/s9", "x26/s10", "x27/s11", "x28/t3", "x29/t4", "x30/t5", "x31/t6"
};
static const std::array<std::string, 10> alu_op_names = {
    "ADD", "SUB", "OR", "AND", "XOR", "SLT", "SLTU", "SLL", "SRL", "SRA"
};
static const std::array<std::string, 8> bu_op_names = {
    "NEVER", "ALWAYS", "EQ", "NE", "LT", "GE", "LTU", "GEU"
};

static const std::array<std::string, 2> alu_a_names = {"RS1", "PC"};
static const std::array<std::string, 2> alu_b_names = {"RS2", "IMM"};
static const std::array<std::string, 9> lsu_ls_names = {
    "NONE", "LOAD B", "LOAD H", "LOAD W", "LOAD UB", "LOAD UH", "STORE B", "STORE H", "STORE W"
};
static const std::array<std::string, 5> reg_wb_names = {"ZERO", "ALU OUT", "PC+4", "LSU", "CSR"};

static const std::map<CsrId, std::string> csr_indices = {
    {CsrId::MTVEC,  "mtvec" },
    {CsrId::MEPC,   "mepc"  },
    {CsrId::MCAUSE, "mcause"},
    {CsrId::MTVAL,  "mtval" },
};

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

const std::string& csr_name(CsrId csr_id) {
    auto v = csr_indices.find(csr_id);
    return v != csr_indices.end() ? v->second : EMPTY;
}
}  // namespace cotm32::name_utils
