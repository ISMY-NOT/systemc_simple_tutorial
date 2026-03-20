#ifndef LOGIC_OR_H
#define LOGIC_OR_H

#include <systemc.h>

// logic or module
SC_MODULE(logic_or){
    // input ports
    sc_in<bool> A;
    sc_in<bool> B;

    // output port
    sc_out<bool> C;

    // process method
    void or_process(){
        C.write(A.read() || B.read());
    }

    // constructor
    SC_CTOR(logic_or){
        SC_METHOD(or_process);
        sensitive << A << B; // sensitivity list
    }
};

#endif /* LOGIC_OR_H */
