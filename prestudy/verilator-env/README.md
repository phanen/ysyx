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

## example

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

## 查看波形
- <https://verilator.org/guide/latest/faq.html?highlight=wave>
- <https://zhuanlan.zhihu.com/p/618184203>

```cc
// verilator need `--trace`
  VerilatedContext *contextp = new VerilatedContext;
  contextp->commandArgs(argc, argv);
  Vtop *top = new Vtop{contextp};

  Verilated::traceEverOn(true);
  VerilatedVcdC *tfp = new VerilatedVcdC;
  top->trace(tfp, 99);   // Trace 99 levels of hierarchy (or see below)
  tfp->dumpvars(1, "t"); // trace 1 level under "t"
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
```

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

## NVBoard
> 虚拟 FPGA


