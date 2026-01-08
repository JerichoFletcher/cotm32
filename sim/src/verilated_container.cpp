#include "verilated_container.hpp"

#include <stdexcept>
#include "Vtop_cotm32.h"
#include "Vtop_processor_core.h"

#include "cotm32_defs.hpp"

VerilatedContainer::VerilatedContainer(int argc, char** argv):
  m_time(0),
  m_started(false),
  m_finished(false) {
  this->m_contextp = std::make_unique<VerilatedContext>();
  this->m_contextp->commandArgs(argc, argv);

  Verilated::traceEverOn(true);
  this->m_tfp = std::make_unique<VerilatedVcdC>();
  this->m_tfp->set_time_unit("ns");
  this->m_tfp->set_time_resolution("ns");

  this->m_top = std::make_unique<Vtop>(this->m_contextp.get());
  this->m_top->trace(this->m_tfp.get(), 99);
  this->m_tfp->open("trace.vcd");
}

VerilatedContainer::~VerilatedContainer() {
  if (this->m_tfp) {
    this->m_tfp->close();
  }
}

void VerilatedContainer::start() {
  if (this->m_started) {
    throw std::runtime_error("Cannot start, container already started");
  }
  if (this->m_finished) {
    throw std::runtime_error("Cannot start, container already finished");
  }

  this->m_started = true;

  this->m_top->i_rst = 1;
  this->tick();
  this->m_top->i_rst = 0;
}

void VerilatedContainer::tick() {
  if (!this->m_started) {
    throw std::runtime_error("Cannot tick, container not started");
  }
  if (this->m_finished) {
    throw std::runtime_error("Cannot tick, container already finished");
  }

  this->m_top->i_clk = 0;
  this->m_top->eval();
  this->m_tfp->dump(this->m_time++);

  this->m_top->i_clk = 1;
  this->m_top->eval();
  this->m_tfp->dump(this->m_time++);
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

void VerilatedContainer::write_byte(uint32_t addr, uint8_t val) {
  if (INST_MEM_START <= addr && addr <= INST_MEM_END) {
    this->m_top->cotm32->core->im__DOT__mem_bytes.m_storage[addr - INST_MEM_START] = val;
  } else if (ROM_MEM_START <= addr && addr <= ROM_MEM_END) {
    this->m_top->cotm32->core->rom__DOT__mem_bytes.m_storage[addr - ROM_MEM_START] = val;
  } else {
    throw std::runtime_error("Error writing byte: address out of valid range");
  }
}
