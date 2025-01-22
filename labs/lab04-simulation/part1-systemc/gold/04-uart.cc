#include<systemc>
using namespace sc_core;
using namespace sc_dt;
using namespace std;

SC_MODULE(UART) {
    sc_in<bool> clk, rst_n;
    sc_in<sc_int<8>> buf;
    sc_in<bool> valid;
    sc_out<bool> ready;
    sc_out<bool> out;
    void uart_thread() {
        ready = 1;
        out   = 1;
        wait();
        while(true) {
            if(valid) {
                ready = 0;
                out   = 0;
                sc_int<8> data = buf;
                wait();
                for(int i = 0; i < 8; i++) {
                    out = data & 1;
                    data = data >> 1;
                    wait();
                }
                out = 1;
                wait();
                ready = 1;
                wait();
            }
            else wait();
        }
    }
    SC_CTOR(UART) {
        SC_THREAD(uart_thread);
        sensitive << clk.pos();
        reset_signal_is(rst_n, 0);
    }
};

int sc_main(int argc, char ** argv) {
    sc_clock clk("clk", 2, SC_NS);
    sc_signal<bool> rst_n("rst_n");
    sc_signal<sc_int<8>> buf("buf");
    sc_signal<bool> out("out");
    sc_signal<bool> ready("ready");
    sc_signal<bool> valid("valid");

    UART uart("mul");
    uart.clk(clk); uart.rst_n(rst_n);
    uart.buf(buf); uart.out(out);
    uart.ready(ready); uart.valid(valid);

    // -------- trace signal --------
    sc_trace_file * f = sc_create_vcd_trace_file(argv[1]);
    f->set_time_unit(1, SC_NS);

    sc_trace(f, clk, clk.name());
    sc_trace(f, rst_n, rst_n.name());
    sc_trace(f, buf, buf.name());
    sc_trace(f, out, out.name());
    sc_trace(f, ready, ready.name());
    sc_trace(f, valid, valid.name());

    // -------- run random test --------
    rst_n = 0;
    valid = 0;
    sc_start(2, SC_NS);
    rst_n = 1;

    for(int i = 0; i < 1000; i++) {
        if(ready && rand() % 5 == 0) {
            buf = rand();
            valid = 1;
            sc_start(2, SC_NS);
            valid = 0;
        }
        else sc_start(2, SC_NS);
    }

    sc_close_vcd_trace_file(f);
    return 0;
}