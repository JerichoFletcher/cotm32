#pragma once

#include <cstdint>

static constexpr inline int NUM_REGS = 32;
static constexpr inline int NUM_CSR = 4;

static constexpr inline int INST_MEM_SIZE = 1024;
static constexpr inline int ROM_MEM_SIZE = 1024;
static constexpr inline int DATA_MEM_SIZE = 4096;

static constexpr inline uint32_t INST_MEM_START = 0x0000'0000;
static constexpr inline uint32_t INST_MEM_END = (INST_MEM_START + INST_MEM_SIZE - 1);
static constexpr inline uint32_t ROM_MEM_START = 0x0001'0000;
static constexpr inline uint32_t ROM_MEM_END = (ROM_MEM_START + ROM_MEM_SIZE - 1);
static constexpr inline uint32_t DATA_MEM_START = 0x1000'0000;
static constexpr inline uint32_t DATA_MEM_END = (DATA_MEM_START + DATA_MEM_SIZE - 1);

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
    RegWritebackSrc_PC4,
    RegWritebackSrc_LSU,
    RegWritebackSrc_CSR,
} RegWritebackSrc;

typedef enum {
    CsrId_MTVEC = 0x305,
    CsrId_MEPC = 0x341,
    CsrId_MCAUSE = 0x342,
    CsrId_MTVAL = 0x343,
} CsrId;
static constexpr CsrId CSR_IDS[4] = {
    CsrId_MTVEC,
    CsrId_MEPC,
    CsrId_MCAUSE,
    CsrId_MTVAL,
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
