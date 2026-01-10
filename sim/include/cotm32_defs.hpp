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
    ADD,
    SUB,
    OR,
    AND,
    XOR,
    SLT,
    SLTU,
    SLL,
    SRL,
    SRA,
} AluOp;

typedef enum { NEVER, ALWAYS, EQ, NE, LT, GE, LTU, GEU } BuOp;

typedef enum {
    RS1,
    PC,
} AluPortA;

typedef enum {
    RS2,
    IMM,
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

typedef enum { ZERO, ALU, PC4, LSU, CSR } RegWritebackSrc;

typedef enum {
    MTVEC = 0x305,
    MEPC = 0x341,
    MCAUSE = 0x342,
    MTVAL = 0x343,
} CsrId;
static constexpr CsrId CSR_IDS[4] = {CsrId::MTVEC, CsrId::MEPC, CsrId::MCAUSE, CsrId::MTVAL};
