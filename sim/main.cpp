#include "Vprocessor_core.h"
#include "verilated.h"

void tick(Vprocessor_core*& top) {
  top->i_clk = 0;
  top->eval();
  top->i_clk = 1;
  top->eval();
}

int main(int argc, char** argv) {
  VerilatedContext* ctx = new VerilatedContext;
  ctx->commandArgs(argc, argv);

  Vprocessor_core* top = new Vprocessor_core{ctx};

  // Initial reset
  top->i_rst = 1;
  tick(top);
  top->i_rst = 0;

  for (int i = 0; i < 200; i++) {
    tick(top);
  }

  delete top;
  delete ctx;

  return 0;
}
