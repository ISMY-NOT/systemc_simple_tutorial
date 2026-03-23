# include<systemc.h>
# include"time_and.h"
# include<iostream>

SC_MODULE(time_and_tb){
    // signal
    sc_signal<bool> clk;
    sc_signal<bool> rst_n;
    sc_signal<bool> A;
    sc_signal<bool> B;
    sc_signal<bool> C;

    // waveform file trace pointer
    sc_trace_file *tf;

    // insistance of tested
    time_and and_inst;

    // clock generation
    void clk_gen(){
        while(true){
            clk.write(0);
            wait(5, SC_NS);
            clk.write(1);
            wait(5, SC_NS);
        }
    }

    void rst_n_gen(){
        rst_n.write(0);
        wait(10, SC_NS);
        rst_n.write(1);
    }

// test process
    void test_process() {
        std::cout << "======= logic and test =======" << endl;
        wait(10, SC_NS);

        // testing
        A = false;
        B = false;
        std::cout << sc_time_stamp() << ": " << "A=" << A << ", B=" << B << ", C=" << C << endl;
        wait(10, SC_NS);

        A = false;
        B = true;
        std::cout << sc_time_stamp() << ": " << "A=" << A << ", B=" << B << ", C=" << C << endl;
        wait(10, SC_NS);

        A = true;
        B = false;
        std::cout << sc_time_stamp() << ": " << "A=" << A << ", B=" << B << ", C=" << C << endl;
        wait(10, SC_NS);

        A = true;
        B = true;
        std::cout << sc_time_stamp() << ": " << "A=" << A << ", B=" << B << ", C=" << C << endl;
        wait(10, SC_NS);

        // test complete
        std::cout << "test have been completed!" << endl;
        std::cout << "waveform file have been saved to logic_and.vcd" << endl;

        sc_stop();
    }

    SC_CTOR(time_and_tb)
    : and_inst("time_and_insitance"){
        // connect to the signals
        and_inst.clk(clk);
        and_inst.rst_n(rst_n);
        and_inst.A(A);
        and_inst.B(B);
        and_inst.C(C);

        SC_THREAD(clk_gen);
        SC_THREAD(rst_n_gen);
        SC_THREAD(test_process);

        tf = sc_create_vcd_trace_file("time_and");
        tf->set_time_unit(1, SC_NS);

        sc_trace(tf, clk, "clk");
        sc_trace(tf, rst_n, "rst_n");
        sc_trace(tf, A, "A");
        sc_trace(tf, B, "B");
        sc_trace(tf, C, "C");
    }

    ~time_and_tb() {
        sc_close_vcd_trace_file(tf);
    }
};


// main
int sc_main(int argc, char* argv[]) {
    time_and_tb tb("testbench");
    sc_start();
    return 0;
}