.phony: all run clean $(patsubst %.cc,run-%,$(wildcard *.cc))

all: $(subst .cc,.out,$(wildcard *.cc))

run: run-$(shell cat .last)
run-%: %.out
	./$*.out $*
	gtkwave $*.vcd

%.out: %.cc
	g++ -lsystemc $^ -o $@
	@echo $* > .last

clean:
	rm -rf *.vcd *.out .last