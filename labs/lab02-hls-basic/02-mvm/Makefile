
WORK_DIR ?= work
CONFIG ?= hls_config.cfg
SRCS    = mvm.cpp mvm.h
TB_SRCS = mvm_tb.cpp $(wildcard *.txt)

COMMON_FLAGS = --work_dir $(WORK_DIR) --mode hls --config $(CONFIG)

csim: $(CONFIG) $(SRCS) $(TB_SRCS)
	vitis-run  	$(COMMON_FLAGS) --csim 
csyn: $(CONFIG) $(SRCS)
	v++  		$(COMMON_FLAGS) -c 
cosim: $(CONFIG) $(SRCS) $(TB_SRCS)
	vitis-run  	$(COMMON_FLAGS) --cosim 
impl: $(CONFIG) $(SRCS)
	vitis-run  	$(COMMON_FLAGS) --impl 
package: $(CONFIG) $(SRCS)
	vitis-run   $(COMMON_FLAGS) --package
clean:
	rm -rf $(WORK_DIR) .Xil *.log compile_commands.json 
