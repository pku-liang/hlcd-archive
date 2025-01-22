WORK_DIR ?= work
CONFIG ?= hls_config.cfg

SRCS    = mm.cpp mm.h
TB_SRCS = mm_tb.cpp data.txt

COMMON_FLAGS = --work_dir $(WORK_DIR) --mode hls --config $(CONFIG)

HLS_REPORT = $(WORK_DIR)/reports/hls_compile.rpt
CSIM_GOOD  = $(WORK_DIR)/csim_good

score: $(CSIM_GOOD) $(HLS_REPORT)
	python3 score.py $(HLS_REPORT)
csim: $(CONFIG) $(SRCS) $(TB_SRCS)
	vitis-run  	$(COMMON_FLAGS) --csim
csyn: $(HLS_REPORT)
$(HLS_REPORT): $(CONFIG) $(SRCS)
	v++  		$(COMMON_FLAGS) -c 
$(CSIM_GOOD): $(CONFIG) $(SRCS) $(TB_SRCS)
	vitis-run  	$(COMMON_FLAGS) --csim
	date > $(CSIM_GOOD)
gen-data: gen-data.py
	python3 $<
data.txt: gen-data.py
	python3 $<
clean:
	rm -rf $(WORK_DIR) .Xil *.log
