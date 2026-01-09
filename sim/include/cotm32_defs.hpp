#pragma once

static constexpr int NUM_REGS       = 32;
static constexpr int NUM_CSR        = 4;

static constexpr int INST_MEM_SIZE  = 1024;
static constexpr int ROM_MEM_SIZE   = 1024;
static constexpr int DATA_MEM_SIZE  = 4096;

static constexpr uint32_t INST_MEM_START  = 0x00000000;
static constexpr uint32_t INST_MEM_END    = (INST_MEM_START + INST_MEM_SIZE - 1);
static constexpr uint32_t ROM_MEM_START   = 0x00010000;
static constexpr uint32_t ROM_MEM_END     = (ROM_MEM_START + ROM_MEM_SIZE - 1);
static constexpr uint32_t DATA_MEM_START  = 0x10000000;
static constexpr uint32_t DATA_MEM_END    = (DATA_MEM_START + DATA_MEM_SIZE - 1);
