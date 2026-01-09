#include "verilated_container.hpp"

#include <cstdio>
#include <stdexcept>
#include "Vtop_cotm32.h"
#include "Vtop_processor_core.h"

#include "cotm32_defs.hpp"

VerilatedContainer::VerilatedContainer(int argc, char** argv):
  m_started(false),
  m_finished(false) {
  this->m_contextp = std::make_unique<VerilatedContext>();
  this->m_contextp->commandArgs(argc, argv);

  this->m_contextp->timeunit(-9);
  this->m_contextp->timeprecision(-9);

  // Verilated::traceEverOn(true);
  // this->m_tfp = std::make_unique<VerilatedVcdC>();
  // this->m_tfp->set_time_unit("ns");
  // this->m_tfp->set_time_resolution("ns");

  this->m_top = std::make_unique<Vtop>(this->m_contextp.get());
  // this->m_top->trace(this->m_tfp.get(), 99);
  // this->m_tfp->open("trace.vcd");
}

VerilatedContainer::~VerilatedContainer() {
  // if (this->m_tfp) {
  //   this->m_tfp->close();
  // }
}

void VerilatedContainer::start() {
  if (this->m_started) {
    throw std::runtime_error("Cannot start, container already started");
  }
  if (this->m_finished) {
    throw std::runtime_error("Cannot start, container already finished");
  }

  this->m_started = true;
  this->reset();
}

void VerilatedContainer::reset() {
  this->m_top->i_rst = 1;
  this->update();
  this->m_top->i_rst = 0;
}

void VerilatedContainer::update() {
  if (!this->m_started) {
    throw std::runtime_error("Cannot tick, container not started");
  }
  if (this->m_finished) {
    throw std::runtime_error("Cannot tick, container already finished");
  }

  this->m_top->i_clk = 0;
  this->m_top->eval();
  this->m_contextp->timeInc(1UL);
  // this->m_tfp->dump(this->m_time++);

  this->m_top->i_clk = 1;
  this->m_top->eval();
  this->m_contextp->timeInc(1UL);
  // this->m_tfp->dump(this->m_time++);
}

void VerilatedContainer::finish() {
  if (!this->m_started) {
    throw std::runtime_error("Cannot finish, container not started");
  }
  if (this->m_finished) {
    throw std::runtime_error("Cannot finish, container already finished");
  }

  this->m_top->final();
  this->m_finished = true;
}

bool VerilatedContainer::read_byte(uint32_t addr, uint8_t* out) const {
  if (INST_MEM_START <= addr && addr <= INST_MEM_END) {
    *out = this->m_top->cotm32->core->im__DOT__mem_bytes[addr - INST_MEM_START];
  } else if (ROM_MEM_START <= addr && addr <= ROM_MEM_END) {
    *out = this->m_top->cotm32->core->rom__DOT__mem_bytes[addr - ROM_MEM_START];
  } else if (DATA_MEM_START <= addr && addr <= DATA_MEM_END) {
    *out = this->m_top->cotm32->core->mem__DOT__mem_bytes[addr - DATA_MEM_START];
  } else {
    return false;
  }
  return true;
}

bool VerilatedContainer::write_byte(uint32_t addr, uint8_t val) {
  if (INST_MEM_START <= addr && addr <= INST_MEM_END) {
    this->m_top->cotm32->core->im__DOT__mem_bytes[addr - INST_MEM_START] = val;
  } else if (ROM_MEM_START <= addr && addr <= ROM_MEM_END) {
    this->m_top->cotm32->core->rom__DOT__mem_bytes[addr - ROM_MEM_START] = val;
  } else if (DATA_MEM_START <= addr && addr <= DATA_MEM_END) {
    this->m_top->cotm32->core->mem__DOT__mem_bytes[addr - DATA_MEM_START] = val;
  } else {
    return false;
  }
  return true;
}
