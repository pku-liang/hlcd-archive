.phony: all show clean

all: trace.vcd

trace.vcd: alu.sv alu_tb.sv tb.cpp
	verilator --sv --cc --exe --trace --build $^
	./obj_dir/Valu

show: trace.vcd
	gtkwave trace.vcd

clean:
	rm -rf obj_dir trace.vcd
