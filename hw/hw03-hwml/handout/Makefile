VERILATOR_FLAGS := -Wall -Wno-UNUSED -Wno-UNDRIVEN -Wno-DECLFILENAME -Wno-EOFNEWLINE -j --trace

.SECONDARY:

obj_dir/V%: obj_dir/V%.h obj_dir/V%.cpp %_tb.cpp common.h
	$(MAKE) -j -C obj_dir -f V$*.mk

obj_dir/V%.h obj_dir/V%.cpp: %.sv
	verilator $(VERILATOR_FLAGS) --cc $^ --exe $*_tb.cpp

run-%: obj_dir/VPEArray%
	@./$<


clean:
	rm -rf obj_dir

.PHONY: clean