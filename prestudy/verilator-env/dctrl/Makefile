# https://verilator.org/guide/latest/example_cc.html#example-c-execution
# --cc to get C++ output (versus e.g., SystemC, or only linting).
# --exe, along with our sim_main.cpp wrapper file, so the build will create an executable instead of only a library.
# --build so Verilator will call make itself. This is we don’t need to manually call make as a separate step. You can also write your own compile rules, and run make yourself as we show in Example SystemC Execution.)
# -j 0 to Verilate using use as many CPU threads as the machine has.
# -Wall so Verilator has stronger lint warnings enabled.
# And finally, our.v which is our SystemVerilog design file.

all:
	bear -- verilator --cc --exe --build --trace -j 0 -Wall --Mdir build sim_main.cpp top.v

clean:
	rm obj_dir/ -rf
