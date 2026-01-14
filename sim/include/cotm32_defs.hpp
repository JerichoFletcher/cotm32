#pragma once

#include <array>
#include <cstdint>

static constexpr inline int NUM_REGS = 32;

static constexpr inline int BOOTROM_MEM_SIZE = 4096;
static constexpr inline int DATA_MEM_SIZE = 4096;

static constexpr inline uint32_t BOOTROM_MEM_START = 0x0000'0000;
static constexpr inline uint32_t BOOTROM_MEM_END = (BOOTROM_MEM_START + BOOTROM_MEM_SIZE - 1);
static constexpr inline uint32_t DATA_MEM_START = 0x8000'0000;
static constexpr inline uint32_t DATA_MEM_END = (DATA_MEM_START + DATA_MEM_SIZE - 1);

//////////////////////////////// RV32I  ////////////////////////////////
typedef enum {
    AluOp_ADD,
    AluOp_SUB,
    AluOp_OR,
    AluOp_AND,
    AluOp_XOR,
    AluOp_SLT,
    AluOp_SLTU,
    AluOp_SLL,
    AluOp_SRL,
    AluOp_SRA,
} AluOp;

typedef enum {
    BuOp_NEVER,
    BuOp_ALWAYS,
    BuOp_EQ,
    BuOp_NE,
    BuOp_LT,
    BuOp_GE,
    BuOp_LTU,
    BuOp_GEU,
} BuOp;

typedef enum {
    AluPortA_RS1,
    AluPortA_PC,
} AluPortA;

typedef enum {
    AluPortB_RS2,
    AluPortB_IMM,
} AluPortB;

typedef enum {
    NONE,
    LOAD_B,
    LOAD_H,
    LOAD_W,
    LOAD_BU,
    LOAD_HU,
    STORE_B,
    STORE_H,
    STORE_W
} LsuLoadStoreOp;

typedef enum {
    RegWritebackSrc_ZERO,
    RegWritebackSrc_ALU,
    RegWriteBackSrc_MU,
    RegWritebackSrc_PC4,
    RegWritebackSrc_LSU,
    RegWritebackSrc_CSR,
} RegWritebackSrc;

typedef enum {
    CsrId_MSTATUS = 0x300,
    CsrId_MIE = 0x304,
    CsrId_MTVEC = 0x305,
    CsrId_MEPC = 0x341,
    CsrId_MCAUSE = 0x342,
    CsrId_MTVAL = 0x343,
    CsrId_MIP = 0x344,
} CsrId;
static constexpr std::array<CsrId, 7> CSR_IDS = {
    CsrId_MSTATUS,
    CsrId_MIE,
    CsrId_MTVEC,
    CsrId_MEPC,
    CsrId_MCAUSE,
    CsrId_MTVAL,
    CsrId_MIP,
};

typedef enum {
    CsrOp_NONE,
    CsrOp_RW,
    CsrOp_RS,
    CsrOp_RC,
} CsrOp;

typedef enum {
    CsrWriteSrc_RS1,
    CsrWriteSrc_IMM,
} CsrWriteSrc;

typedef enum {
    FwdSrc_NONE,
    FwdSrc_EXMEM,
    FwdSrc_MEMWB,
} FwdSrc;

//////////////////////////////// RV32M  ////////////////////////////////
typedef enum {
    MuOp_NOP,
    MuOp_MUL,
    MuOp_MULH,
    MuOp_MULHU,
    MuOp_MULHSU,
    MuOp_DIV,
    MuOp_DIVU,
    MuOp_REM,
    MuOp_REMU,
} MuOp;
