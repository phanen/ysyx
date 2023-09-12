#include <Vtop.h>
#include <nvboard.h>
#include <stdio.h>
#include <stdlib.h>

static TOP_NAME dut;

void nvboard_bind_all_pins(Vtop *top);

static void single_cycle() {
  dut.clk = 0;
  dut.eval();
  // sleep(1);
  dut.clk = 1;
  dut.eval();
  // sleep(1);
}

void reset(int n) {
  dut.rst = 1;
  while (n-- > 0)
    single_cycle();
  dut.rst = 0;
}

int main() {
  nvboard_bind_all_pins(&dut);
  nvboard_init();

  reset(10);

  const size_t led_period = 5000000;
  for (size_t i = 0; i < led_period * 4; ++i) {
    nvboard_update();
    single_cycle();
    // single_cycle();
    // single_cycle();
  }
  nvboard_quit();
}
