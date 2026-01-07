#include "vtop_adapter.hpp"

#include <stdexcept>
#include <cstdio>
#include "Vtop_cotm32.h"
#include "Vtop_processor_core.h"

#include "cotm32_defs.hpp"

VtopAdapter VtopAdapter::from_contextp(VerilatedContext *contextp) {
  return VtopAdapter(std::unique_ptr<Vtop>{new Vtop{contextp}});
}

VtopAdapter::VtopAdapter(std::unique_ptr<Vtop> top) {
  if (top == nullptr) {
    throw std::runtime_error("top is null");
  }
  this->top_ptr = std::move(top);
}

VtopAdapter::~VtopAdapter() {
  this->top_ptr->final();
  this->top_ptr.release();
}

const std::unique_ptr<Vtop>& VtopAdapter::ptr() const {
  return this->top_ptr;
}

void VtopAdapter::tick_half() {
  this->top_ptr->i_clk = 1 - this->top_ptr->i_clk;
  this->top_ptr->eval();
}

IData VtopAdapter::pc() const {
  return this->top_ptr->cotm32->core->pc;
}

IData VtopAdapter::inst() const {
  return this->top_ptr->cotm32->core->inst;
}

void VtopAdapter::write_byte(uint32_t addr, uint8_t val) {
  if (INST_MEM_START <= addr && addr <= INST_MEM_END) {
    this->top_ptr->cotm32->core->im__DOT__mem_bytes.m_storage[addr - INST_MEM_START] = val;
  } else if (ROM_MEM_START <= addr && addr <= ROM_MEM_END) {
    this->top_ptr->cotm32->core->rom__DOT__mem_bytes.m_storage[addr - ROM_MEM_START] = val;
  } else {
    throw std::runtime_error("Error writing byte: address out of valid range");
  }
}
