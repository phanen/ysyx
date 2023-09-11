# 搭建 verilator 仿真环境
- <https://ysyx.oscc.cc/docs/2306/prestudy/0.4.html>

## what is verilator
- <https://www.veripool.org/verilator>
- <https://verilator.org/guide/latest>

the fastest Verilog/SystemVerilog simulator
- Verilog/SystemVerilog -> multithreaded C++/SystemC
- parameters similar to GCC or Synopsys's VCS
- optionally inserting assertion checks and coverage-analysis points
```sh
yay -S verilator gtkwave iverilog
```

iverilog
- <https://github.com/steveicarus/iverilog>
- <https://bleyer.org/icarus>

## verilator example

<https://verilator.org/guide/latest/example_cc.html#example-c-execution>

```sh
verilator --cc --exe --build -j 0 -Wall sim_main.cpp our.v
```
- `--cc` to get C++ output (versus e.g., SystemC, or only linting).
- `--exe`, along with our `sim_main.cpp` wrapper file, so the build will create an executable instead of only a library.
- `--build` so Verilator will call make itself. This is we don’t need to manually call make as a separate step. You can also write your own compile rules, and run make yourself as we show in Example SystemC Execution.)
- `-j 0` to Verilate using use as many CPU threads as the machine has.
- `-Wall` so Verilator has stronger lint warnings enabled.
And finally, our.v which is our SystemVerilog design file.

和 iverilog 输出基本一样
![img](https://i.imgur.com/zZ9m5b9.png)

##  双控开关模块仿真

照葫芦画瓢
```
verilator --cc --exe --build -j 0 -Wall sim_main.cpp top.v
```
- 生成头 "Vtop.h"


生成波形文件
- <https://verilator.org/guide/latest/faq.html?highlight=wave>

`bash init.sh nemu` -> `make sim`
> 必须先初始化 nemu..
![img:nemu](https://i.imgur.com/l1puBw1.png)
```make
YSYX_HOME = $(NEMU_HOME)/..

# prototype: git_commit(msg)
define git_commit
    -@flock $(LOCK_DIR) $(MAKE) -C $(YSYX_HOME) .git_commit MSG='$(1)'
    -@sync
endef
```

## gtkwave 波形
- <https://verilator.org/guide/latest/faq.html?highlight=wave>
- <https://zhuanlan.zhihu.com/p/618184203>

```cc
// verilator need `--trace`
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

    contextp->timeInc(1);
    tfp->dump(contextp->time());
  }
  tfp->close();
```

```sh
./build/Vtop && gtkwave ./build/wave.vcd
```
![img:wave](https://i.imgur.com/vfoy7L9.png)

## 理解 verilator 的 RTL 仿真行为

- top.v 生成 硬件仿真的 C++ 工程(库函数形式, Vtop.h 等)
- sim_main.cpp 调用硬件库, 编译完成仿真

Vtop
```cc
class alignas(VL_CACHE_LINE_BYTES) Vtop VL_NOT_FINAL : public VerilatedModel {
  private:
    // Symbol table holding complete model state (owned by this class)
    Vtop__Syms* const vlSymsp;

  public:
    // PORTS
    VL_IN8(&a,0,0);
    VL_IN8(&b,0,0);
    VL_OUT8(&f,0,0);

    // Root instance pointer to allow access to model internals,
    // including inlined /* verilator public_flat_* */ items.
    Vtop___024root* const rootp;

    explicit Vtop(VerilatedContext* contextp, const char* name = "TOP");
    explicit Vtop(const char* name = "TOP");
    virtual ~Vtop();
  private:
    VL_UNCOPYABLE(Vtop);  ///< Copying not allowed

  public:
    /// Evaluate the model.  Application must call when inputs change.
    void eval() { eval_step(); }
    /// Evaluate when calling multiple units/models per time step.
    void eval_step();
    /// Evaluate at end of a timestep for tracing, when using eval_step().
    /// Application must call after all eval() and before time changes.
    void eval_end_step() {}
    /// Simulation complete, run final blocks.  Application must call on completion.
    void final();
    /// Are there scheduled events to handle?
    bool eventsPending();
    /// Returns time at next time slot. Aborts if !eventsPending()
    uint64_t nextTimeSlot();
    /// Trace signals in the model; called by application code
    void trace(VerilatedVcdC* tfp, int levels, int options = 0);
    /// Retrieve name of this model instance (as passed to constructor).
    const char* name() const;

    // Abstract methods from VerilatedModel
    const char* hierName() const override final;
    const char* modelName() const override final;
    unsigned threads() const override final;
    std::unique_ptr<VerilatedTraceConfig> traceConfig() const override final;
};
```

## NVBoard 模拟开发板
> Nju Virual Board? 模拟虚拟的 FPGA

demo
```
├── constr
│   └── top.nxdc
├── csrc
│   └── main.cpp
├── Makefile
├── resource
│   └── picture.hex
└── vsrc
    ├── led.v
    ├── ps2_keyboard.v
    ├── seg.v
    ├── top.v
    └── vga_ctrl.v
```

makefile
```make
VERILATOR_CFLAGS += -MMD --build -cc  \
				-O3 --x-assign fast --x-initial fast --noassert

# constraint file
SRC_AUTO_BIND = $(abspath $(BUILD_DIR)/auto_bind.cpp)
$(SRC_AUTO_BIND): $(NXDC_FILES)
	python3 $(NVBOARD_HOME)/scripts/auto_pin_bind.py $^ $@

# project source
VSRCS = $(shell find $(abspath ./vsrc) -name "*.v")
CSRCS = $(shell find $(abspath ./csrc) -name "*.c" -or -name "*.cc" -or -name "*.cpp")
CSRCS += $(SRC_AUTO_BIND)

# rules for NVBoard
include $(NVBOARD_HOME)/scripts/nvboard.mk

# rules for verilator
INCFLAGS = $(addprefix -I, $(INC_PATH))
CFLAGS += $(INCFLAGS) -DTOP_NAME="\"V$(TOPNAME)\""
LDFLAGS += -lSDL2 -lSDL2_image

$(BIN): $(VSRCS) $(CSRCS) $(NVBOARD_ARCHIVE)
	@rm -rf $(OBJ_DIR)
	$(VERILATOR) $(VERILATOR_CFLAGS) \
		--top-module $(TOPNAME) $^ \
		$(addprefix -CFLAGS , $(CFLAGS)) $(addprefix -LDFLAGS , $(LDFLAGS)) \
		--Mdir $(OBJ_DIR) --exe -o $(abspath $(BIN))
```

## 
