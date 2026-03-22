#ifndef TIME_AND
#define TIME_AND

# include <systemc.h>

SC_MODULE(time_and){
    // input ports
    sc_in<bool> clk;
    sc_in<bool> rst_n;
    sc_in<bool> A;
    sc_in<bool> B;

    // output port
    sc_out<bool> C;

    // time and gate process
    void time_and_process(){
        while (true) {
            wait();
            C.write(A.read() && B.read());
        }
    }

    // constructor
    SC_CTOR(time_and){
        SC_CTHREAD(time_and_process, clk.pos());
        reset_signal_is(rst_n, false);
    }
};

#endif /* TIME_AND */
