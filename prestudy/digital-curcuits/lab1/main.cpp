#include "Vtop.h"
#include "verilated.h"
#include "verilated_vcd_c.h"

VerilatedContext *contextp = NULL;
VerilatedVcdC *tfp = NULL;

static Vtop *top;

void step_and_dump_wave() {
  top->eval();
  contextp->timeInc(1);
  tfp->dump(contextp->time());
}
void sim_init() {
  contextp = new VerilatedContext;
  tfp = new VerilatedVcdC;
  top = new Vtop;
  contextp->traceEverOn(true);
  top->trace(tfp, 0);
  tfp->open(WAVEFILE);
}

void sim_exit() {
  step_and_dump_wave();
  tfp->close();
}

int main() {
  sim_init();

  top->s = 0;
  top->a = 0;
  top->b = 0;
  step_and_dump_wave();
  top->b = 1;
  step_and_dump_wave();
  top->a = 1;
  top->b = 0;
  step_and_dump_wave();
  top->b = 1;
  step_and_dump_wave();
  top->s = 1;
  top->a = 0;
  top->b = 0;
  step_and_dump_wave();
  top->b = 1;
  step_and_dump_wave();
  top->a = 1;
  top->b = 0;
  step_and_dump_wave();
  top->b = 1;
  step_and_dump_wave();
  sim_exit();
}
