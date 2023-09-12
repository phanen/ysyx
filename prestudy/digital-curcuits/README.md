# nju lab
- <https://nju-projectn.github.io/dlco-lecture-note/index.html>
- <https://pages.hmc.edu/harris/ddca/ddcarv.html>
- <https://hdlbits.01xz.net/wiki/Main_Page>
- <https://verilogoj.ustc.edu.cn/oj/>
- <https://inst.eecs.berkeley.edu/~cs150/fa06/Labs/verilog-ieee.pdf>

FPGA 的主要作用是仿真加速, 而 ysyx 基本上遇不到:
```
FPGA_syn_time + FPGA_impl_time + FPGA_run_time < verilator_compile_time + verilator_run_time
FPGA_syn_time + FPGA_impl_time + FPGA_run_time < verilator_compile_time + verilator_run_time
```


## lab1

建模方式
- 数据流建模: 画电路
- 结构化建模: 逐层画电路
- 行为建模: 编程, 电路转化为事件队列

行为建模
- 在硬件描述语言中, "执行" 的精确含义是什么?
- 是谁在执行 Verilog 的语句?  是电路, 综合器, 还是其它的?
- if 的条件满足, 就不执行 else 后的语句, 这里的 "不执行" 又是什么意思?  和描述电路有什么联系?
- 有 "并发执行", 又有 "顺序执行", 还有 "任何一个变量发生变化就立即执行", 以及 "在任何情况下都执行", 它们都是如何在设计出来的电路中体现的?


case 语句中建议无论如何保留 default 选项
- 如果选择列表中没有包含选择表达式的所有选项, 而此时又没有 default 选项的话, 综合器会综合出一个锁存器以保存未被覆盖的情况下输出的过去值, 这一般是不希望出现的情况
