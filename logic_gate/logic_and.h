#ifndef LOGIC_AND_H
#define LOGIC_AND_H

# include <systemc.h>

// logic and module
SC_MODULE(logic_and){
    // input ports
    sc_in<bool> A;
    sc_in<bool> B;

    // output port
    sc_out<bool> C;

    // and process
    void and_process(){
        C.write(A.read() && B.read());
    }

    // constructor
    SC_CTOR(logic_and){
        SC_METHOD(and_process);
        sensitive << A <<  B; // sensitive list
    }
};

#endif /* LOGIC_AND_H */
