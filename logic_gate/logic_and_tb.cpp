# include <systemc.h>
# include <iostream>
# include "logic_and.h"

SC_MODULE(logic_and_tb){
    // signals
    sc_signal<bool> A_sig;
    sc_signal<bool> B_sig;
    sc_signal<bool> C_sig;

    // waveform trace file pointer
    sc_trace_file *tf;

    // insistance of tested
    logic_and and_inst;

    // test process
    void test_process() {
        std::cout << "======= logic and test =======" << endl;
        wait(10, SC_NS);

        // testing
        A_sig = false;
        B_sig = false;
        std::cout << sc_time_stamp() << ": " << "A=" << A_sig << ", B=" << B_sig << ", C=" << C_sig << endl;
        wait(10, SC_NS);

        A_sig = false;
        B_sig = true;
        std::cout << sc_time_stamp() << ": " << "A=" << A_sig << ", B=" << B_sig << ", C=" << C_sig << endl;
        wait(10, SC_NS);

        A_sig = true;
        B_sig = false;
        std::cout << sc_time_stamp() << ": " << "A=" << A_sig << ", B=" << B_sig << ", C=" << C_sig << endl;
        wait(10, SC_NS);

        A_sig = true;
        B_sig = true;
        std::cout << sc_time_stamp() << ": " << "A=" << A_sig << ", B=" << B_sig << ", C=" << C_sig << endl;
        wait(10, SC_NS);

        // test complete
        std::cout << "test have been completed!" << endl;
        std::cout << "waveform file have been saved to logic_and.vcd" << endl;

        sc_stop();
    }

    // constuctor
    SC_CTOR(logic_and_tb)
    : and_inst("and_instance") {
        // connect to the signals
        and_inst.A(A_sig);
        and_inst.B(B_sig);
        and_inst.C(C_sig);

        // register the thread
        SC_THREAD(test_process);

        // generate the waveform file
        tf = sc_create_vcd_trace_file("logic_and");

        // add the signals to waveform file
        sc_trace(tf, A_sig, "A");
        sc_trace(tf, B_sig, "B");
        sc_trace(tf, C_sig, "C");
    }

    ~logic_and_tb() {
        sc_close_vcd_trace_file(tf);
    }
};

// main
int sc_main(int argc, char* argv[]) {
    logic_and_tb tb("testbench");
    sc_start();
    return 0;
}