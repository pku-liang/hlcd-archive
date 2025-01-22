#include<systemc>
using namespace sc_core;
using namespace sc_dt;
using namespace std;

SC_MODULE(adder) {
    sc_in<int> a, b;
    sc_out<int> sum;
    void add() {
        // CODE HERE!
    }
    SC_CTOR(adder) {
        // CODE HERE!
    }
};

int sc_main(int argc, char ** argv) {
    // -------- define signal --------
    sc_signal<int> a("a");
    sc_signal<int> b("b");
    sc_signal<int> sum("sum");

    // -------- define module --------
    adder add("adder");
    add.a(a); add.b(b); add.sum(sum);

    // -------- trace signal --------
    auto f = sc_create_vcd_trace_file(argv[1]);
    sc_trace(f, a, a.name());
    sc_trace(f, b, b.name());
    sc_trace(f, sum, sum.name());

    // -------- run random test --------
    for(int i = 0; i < 100; i++) {
        a = rand(); b = rand();
        sc_start(1, SC_PS);
    }

    sc_close_vcd_trace_file(f);
    return 0;
}

