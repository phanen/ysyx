#include <Vtop.h>
#include <nvboard.h>
#include <stdio.h>
#include <stdlib.h>

static TOP_NAME dut;

void nvboard_bind_all_pins(Vtop *top);

static void single_cycle() {
  dut.a = rand() & 1;
  dut.b = rand() & 1;
  dut.eval();
  sleep(1);
}

int main() {
  nvboard_bind_all_pins(&dut);
  nvboard_init();

  for (size_t i = 0; i < 10; ++i) {
    nvboard_update();
    single_cycle();
  }
  nvboard_quit();
}
