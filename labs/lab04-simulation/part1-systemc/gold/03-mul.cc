#include<systemc>
#include<queue>
using namespace sc_core;
using namespace sc_dt;
using namespace std;

SC_MODULE(Mul) {
    sc_in<bool> clk, rst_n;
    sc_in<int> a, b;
    sc_out<int> out;
    deque<int> queue{0,0,0};
    void mul() {
        out = 0;
        wait();
        while(true) {
            out = queue.front();
            queue.pop_front();
            queue.push_back(a * b);
            wait();
        }
    }
    SC_CTOR(Mul) {
        SC_THREAD(mul);
        sensitive << clk.pos();
        reset_signal_is(rst_n, 0);
    }
};

int sc_main(int argc, char ** argv) {
    sc_clock clk("clk", 2, SC_NS);
    sc_signal<bool> rst_n("rst_n");
    sc_signal<int> a("a"), b("b"), out("out");

    Mul mul("mul");
    mul.clk(clk); mul.rst_n(rst_n); mul.a(a); mul.b(b); mul.out(out);

    // -------- trace signal --------
    sc_trace_file * f = sc_create_vcd_trace_file(argv[1]);
    f->set_time_unit(1, SC_NS);

    sc_trace(f, clk, clk.name());
    sc_trace(f, rst_n, rst_n.name());
    sc_trace(f, a, a.name());
    sc_trace(f, b, b.name());
    sc_trace(f, out, out.name());

    // -------- run random test --------
    rst_n = 0;
    sc_start(2, SC_NS);
    rst_n = 1;

    for(int i = 0; i < 100; i++) {
        a = rand() % 11; b = rand() % 11;
        sc_start(2, SC_NS);
    }

    sc_close_vcd_trace_file(f);
    return 0;
}