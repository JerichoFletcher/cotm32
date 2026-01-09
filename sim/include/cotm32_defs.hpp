#pragma once

static constexpr inline int NUM_REGS = 32;
static constexpr inline int NUM_CSR = 4;

static constexpr inline int INST_MEM_SIZE = 1024;
static constexpr inline int ROM_MEM_SIZE = 1024;
static constexpr inline int DATA_MEM_SIZE = 4096;

static constexpr inline uint32_t INST_MEM_START = 0x00000000;
static constexpr inline uint32_t INST_MEM_END = (INST_MEM_START + INST_MEM_SIZE - 1);
static constexpr inline uint32_t ROM_MEM_START = 0x00010000;
static constexpr inline uint32_t ROM_MEM_END = (ROM_MEM_START + ROM_MEM_SIZE - 1);
static constexpr inline uint32_t DATA_MEM_START = 0x10000000;
static constexpr inline uint32_t DATA_MEM_END = (DATA_MEM_START + DATA_MEM_SIZE - 1);
