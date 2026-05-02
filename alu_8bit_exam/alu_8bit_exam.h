// alu_8bit_exam.h
// 8位带符号补码 ALU（单文件版）
// 支持：加、减、取反、与、或、异或、比较、小于、等于、乘、除

#ifndef ALU_8BIT_EXAM_H
#define ALU_8BIT_EXAM_H

#include <systemc.h>

SC_MODULE(alu_8bit_exam) {
    // ==================== 输入端口 ====================
    sc_in<bool>          clk;
    sc_in<bool>          rst_n;
    sc_in<sc_int<8>>     A;
    sc_in<sc_int<8>>     B;
    sc_in<sc_uint<32>>   op;        // 实际只使用低4位

    // ==================== 输出端口 ====================
    sc_out<sc_int<8>>    result;
    sc_out<bool>         zero;
    sc_out<bool>         overflow;
    sc_out<bool>         carry;

    // ==================== ALU 处理进程 ====================
    void alu_process();

    // ==================== 构造函数 ====================
    SC_CTOR(alu_8bit_exam) {
        SC_CTHREAD(alu_process, clk.pos());
        reset_signal_is(rst_n, false);   // 声明低电平复位
    }
};

#endif // ALU_8BIT_EXAM_H