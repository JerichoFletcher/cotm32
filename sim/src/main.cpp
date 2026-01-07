#include "verilated.h"
#include "verilated_vcd_c.h"
#include "Vtop.h"
#include "Vtop_cotm32.h"
#include "Vtop_processor_core.h"

#include <cstdio>

#include "vtop_adapter.hpp"
#include "load_elf.hpp"

bool wait_until_key(int key) {
  int c;
  while ((c = std::getchar()) != '\n' && c != EOF && c != key);
  return c == key;
}

int main(int argc, char** argv) {
  Verilated::debug(0);
  const std::unique_ptr<VerilatedContext> contextp{new VerilatedContext};
  contextp->commandArgs(argc, argv);

  Verilated::traceEverOn(true);
  std::unique_ptr<VerilatedVcdC> tfp(new VerilatedVcdC);
  tfp->set_time_resolution("ns");
  tfp->set_time_unit("ns");

  {
    auto top = VtopAdapter::from_contextp(contextp.get());
    top.ptr()->trace(tfp.get(), 99);
    tfp->open("trace.vcd");
    tfp->dump(0);
    
#ifdef BOOT_ROM_PATH
    load_elf(BOOT_ROM_PATH, top);
    top.ptr()->eval();
#endif

    int t = 1;
    top.ptr()->i_rst = 1;
    top.tick_half();
    tfp->dump(t++);

    top.ptr()->i_rst = 0;
    top.tick_half();
    tfp->dump(t++);
  
    for (;; t++) {
      top.tick_half();
      tfp->dump(t);

      std::printf("[T%04d] CLK=%1d PC=%08x INST=%08x\n", t, top.ptr()->i_clk, top.pc(), top.inst());
      if (wait_until_key('q')) break;
    }

  }
  
  tfp->close();
  return 0;
}
