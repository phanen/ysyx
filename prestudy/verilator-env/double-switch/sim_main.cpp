#include "Vtop.h"
#include "verilated.h"
#include "verilated_vcd_c.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

  // const std::unique_ptr<VerilatedContext> contextp{new VerilatedContext};
  VerilatedContext *contextp = new VerilatedContext;
  contextp->commandArgs(argc, argv);
  Vtop *top = new Vtop{contextp};

  VerilatedVcdC *tfp = new VerilatedVcdC;

  Verilated::traceEverOn(true);
  top->trace(tfp, 99); // Trace 99 levels of hierarchy (or see below)
  // tfp->dumpvars(1, "t"); // trace 1 level under "t"
  tfp->open("./build/wave.vcd");

  size_t sim_time = 100;
  while (contextp->time() < sim_time && !contextp->gotFinish()) {
    int a = rand() & 1;
    int b = rand() & 1;
    top->a = a;
    top->b = b;
    top->eval();
    printf("a = %d, b = %d, f = %d\n", a, b, top->f);
    assert(top->f == (a ^ b));

    contextp->timeInc(1);
    tfp->dump(contextp->time());
  }
  tfp->close();
  return 0;
}
