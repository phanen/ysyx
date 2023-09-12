some notes

## debug

### verilator

escape
```sh
verilator -CFLAG "xxx"
# backend
g++ xxx

verilator -CFLAG \"xxx\"
# backend
g++ -CFLAG \"xxx\"
```

### verilog

```verilog
module top (
    // ...
    output [15:0] led, // error comma
);
```

## todo

- [ ] nvboard: sw 用鼠标拖似乎相当难用 (如果 `sleep` 基本失灵)
- [ ] verilator: 理解仿真过程
