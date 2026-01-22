#include "name_utils.hpp"

#include <array>
#include <map>

#include "cotm32_defs.hpp"

template <size_t N>
using StringArray = std::array<std::string, N>;

template <typename T>
using StringMap = std::map<T, std::string>;

static const std::string EMPTY;

static const StringArray<NUM_REGS> reg_names = {
    "x0/zero", "x1/ra",  "x2/sp",   "x3/gp",   "x4/tp",  "x5/t0",  "x6/t1",  "x7/t2",
    "x8/s0",   "x9/s1",  "x10/a0",  "x11/a1",  "x12/a2", "x13/a3", "x14/a4", "x15/a5",
    "x16/a6",  "x17/a7", "x18/s2",  "x19/s3",  "x20/s4", "x21/s5", "x22/s6", "x23/s7",
    "x24/s8",  "x25/s9", "x26/s10", "x27/s11", "x28/t3", "x29/t4", "x30/t5", "x31/t6"
};
static const StringMap<AluOp> alu_op_names = {
    {AluOp::AluOp_ADD,  "ADD" },
    {AluOp::AluOp_SUB,  "SUB" },
    {AluOp::AluOp_OR,   "OR"  },
    {AluOp::AluOp_AND,  "AND" },
    {AluOp::AluOp_XOR,  "XOR" },
    {AluOp::AluOp_SLT,  "SLT" },
    {AluOp::AluOp_SLTU, "SLTU"},
    {AluOp::AluOp_SLL,  "SLL" },
    {AluOp::AluOp_SRL,  "SRL" },
    {AluOp::AluOp_SRA,  "SRA" },
};
static const StringMap<BuOp> bu_op_names = {
    {BuOp::BuOp_NEVER,  "NEVER" },
    {BuOp::BuOp_ALWAYS, "ALWAYS"},
    {BuOp::BuOp_EQ,     "EQ"    },
    {BuOp::BuOp_NE,     "NE"    },
    {BuOp::BuOp_LT,     "LT"    },
    {BuOp::BuOp_GE,     "GE"    },
    {BuOp::BuOp_LTU,    "LTU"   },
    {BuOp::BuOp_GEU,    "GEU"   },
};

static const StringMap<AluPortA> alu_a_names = {
    {AluPortA::AluPortA_RS1, "RS1"},
    {AluPortA::AluPortA_PC,  "PC" },
};
static const StringMap<AluPortB> alu_b_names = {
    {AluPortB::AluPortB_RS2, "RS2"},
    {AluPortB::AluPortB_IMM, "IMM"},
};
static const StringMap<LsuLoadStoreOp> lsu_ls_names = {
    {LsuLoadStoreOp::LsuLoadStoreOp_NONE,    "NONE"   },
    {LsuLoadStoreOp::LsuLoadStoreOp_LOAD_B,  "LOAD B" },
    {LsuLoadStoreOp::LsuLoadStoreOp_LOAD_H,  "LOAD H" },
    {LsuLoadStoreOp::LsuLoadStoreOp_LOAD_W,  "LOAD W" },
    {LsuLoadStoreOp::LsuLoadStoreOp_LOAD_BU, "LOAD UB"},
    {LsuLoadStoreOp::LsuLoadStoreOp_LOAD_HU, "LOAD UH"},
    {LsuLoadStoreOp::LsuLoadStoreOp_STORE_B, "STORE B"},
    {LsuLoadStoreOp::LsuLoadStoreOp_STORE_H, "STORE H"},
    {LsuLoadStoreOp::LsuLoadStoreOp_STORE_W, "STORE W"},
};
static const StringMap<RegWritebackSrc> reg_wb_names = {
    {RegWritebackSrc::RegWritebackSrc_ZERO, "ZERO"   },
    {RegWritebackSrc::RegWritebackSrc_ALU,  "ALU OUT"},
    {RegWritebackSrc::RegWriteBackSrc_MU,   "MU OUT" },
    {RegWritebackSrc::RegWritebackSrc_PC4,  "PC+4"   },
    {RegWritebackSrc::RegWritebackSrc_LSU,  "LSU"    },
    {RegWritebackSrc::RegWritebackSrc_CSR,  "CSR"    },
};

static const StringMap<MuOp> mu_op_names = {
    {MuOp::MuOp_NOP,    "NOP"   },
    {MuOp::MuOp_MUL,    "MUL"   },
    {MuOp::MuOp_MULH,   "MULH"  },
    {MuOp::MuOp_MULHU,  "MULHU" },
    {MuOp::MuOp_MULHSU, "MULHSU"},
    {MuOp::MuOp_DIV,    "DIV"   },
    {MuOp::MuOp_DIVU,   "DIVU"  },
    {MuOp::MuOp_REM,    "REM"   },
    {MuOp::MuOp_REMU,   "REMU"  },
};

static const StringMap<PrivMode> priv_mode_names = {
    {PrivMode::PrivMode_U, "User"   },
    {PrivMode::PrivMode_M, "Machine"},
};

static const StringMap<CsrId> csr_names = {
    {CsrId::CsrId_MSTATUS,  "mstatus" },
    {CsrId::CsrId_MIE,      "mie"     },
    {CsrId::CsrId_MTVEC,    "mtvec"   },
    {CsrId::CsrId_MSCRATCH, "mscratch"},
    {CsrId::CsrId_MEPC,     "mepc"    },
    {CsrId::CsrId_MCAUSE,   "mcause"  },
    {CsrId::CsrId_MTVAL,    "mtval"   },
    {CsrId::CsrId_MIP,      "mip"     },
};
static const StringMap<CsrWriteSrc> csr_write_names = {
    {CsrWriteSrc::CsrWriteSrc_RS1, "RS1"},
    {CsrWriteSrc::CsrWriteSrc_IMM, "IMM"},
};
static const StringMap<CsrOp> csr_op_names = {
    {CsrOp::CsrOp_NONE, "NONE"},
    {CsrOp::CsrOp_RW,   "RW"  },
    {CsrOp::CsrOp_RS,   "RS"  },
    {CsrOp::CsrOp_RC,   "RC"  },
};

static const StringMap<FwdSrc> fwd_src_names = {
    {FwdSrc::FwdSrc_NONE,  "NONE"  },
    {FwdSrc::FwdSrc_EXMEM, "EX/MEM"},
    {FwdSrc::FwdSrc_MEMWB, "MEM/WB"},
};

template <typename T, std::size_t cap>
inline bool in_bounds(int i, const std::array<T, cap>& arr) {
    return 0 <= i && i < cap;
}

template <typename T>
inline const std::string& get_or_empty(const StringMap<T>& map, const T& key) {
    auto v = map.find(key);
    return v != map.end() ? v->second : EMPTY;
}

namespace cotm32::name_utils {
const std::string& reg_name(uint8_t reg_index) {
    if (!in_bounds(reg_index, reg_names)) return EMPTY;
    return reg_names[reg_index];
}

const std::string& alu_op_name(AluOp alu_op) { return get_or_empty(alu_op_names, alu_op); }

const std::string& bu_op_name(BuOp bu_op) { return get_or_empty(bu_op_names, bu_op); }

const std::string& alu_a_name(AluPortA alu_a_sel) { return get_or_empty(alu_a_names, alu_a_sel); }

const std::string& alu_b_name(AluPortB alu_b_sel) { return get_or_empty(alu_b_names, alu_b_sel); }

const std::string& lsu_ls_op_name(LsuLoadStoreOp lsu_ls_op) {
    return get_or_empty(lsu_ls_names, lsu_ls_op);
}

const std::string& reg_wb_name(RegWritebackSrc reg_wb_sel) {
    return get_or_empty(reg_wb_names, reg_wb_sel);
}

const std::string& mu_op_name(MuOp mu_op) { return get_or_empty(mu_op_names, mu_op); }

const std::string& priv_mode_name(PrivMode priv_mode) {
    return get_or_empty(priv_mode_names, priv_mode);
}

const std::string& csr_name(CsrId csr_id) { return get_or_empty(csr_names, csr_id); }

const std::string& csr_write_name(CsrWriteSrc csr_write) {
    return get_or_empty(csr_write_names, csr_write);
}

const std::string& csr_op_name(CsrOp csr_op) { return get_or_empty(csr_op_names, csr_op); }

const std::string& pipe_fwd_src_name(FwdSrc fwd_src) {
    return get_or_empty(fwd_src_names, fwd_src);
}
}  // namespace cotm32::name_utils
