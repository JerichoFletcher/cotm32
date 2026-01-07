#pragma once
#include "verilated.h"
#include "Vtop.h"

class VtopAdapter {
  public:
    static VtopAdapter from_contextp(VerilatedContext *contextp);
    ~VtopAdapter();

    const std::unique_ptr<Vtop>& ptr() const;

    void tick_half();

    IData pc() const;
    IData inst() const;
    
    void write_byte(uint32_t addr, uint8_t val);
  
  private:
    VtopAdapter(std::unique_ptr<Vtop> top);

    std::unique_ptr<Vtop> top_ptr;
};
