.phony: all stat show clean

all: cov.dat cov.info

cov.dat cov.info &: alu.sv alu_tb.sv tb.cpp
	verilator -cc --build --exe --coverage $^
	./obj_dir/Valu
	verilator_coverage cov.dat -write-info cov.info

stat: cov.info
	lcov --summary cov.info

show: cov.info
	genhtml cov.info -o obj_dir
	npx light-server -s obj_dir

clean:
	rm -rf obj_dir cov.dat cov.info