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
    void alu_process() {
        // 复位时的初始值
        result.write(0);
        zero.write(true);
        overflow.write(false);
        carry.write(false);

        while (true) {
            wait();                     // 等待时钟上升沿

            if (!rst_n.read()) {        // 低电平复位
                result.write(0);
                zero.write(true);
                overflow.write(false);
                carry.write(false);
                continue;
            }

            sc_int<8>  a_val   = A.read();
            sc_int<8>  b_val   = B.read();
            sc_uint<4> op_code = op.read().range(3, 0);

            wait();                     // 等待时钟上升沿

            sc_int<8>  res       = 0;
            bool       ov_flag   = false;
            bool       cy_flag   = false;

            switch (op_code) {
                case 0: // 加法 A + B
                {
                    sc_int<9> sum = (sc_int<9>)a_val + (sc_int<9>)b_val;
                    res = sum.range(7, 0);
                    ov_flag = (sum > 127) || (sum < -128);
                    cy_flag = sum[8];                    // 进位
                }
                break;

                case 1: // 减法 A - B
                {
                    sc_int<9> diff = (sc_int<9>)a_val - (sc_int<9>)b_val;
                    res = diff.range(7, 0);
                    ov_flag = (diff > 127) || (diff < -128);
                    cy_flag = diff[8];                   // 借位标志（这里简化为进位位）
                }
                break;

                case 2: // 取反 ~A
                    res = ~a_val;
                    break;

                case 3: // 与 A & B
                    res = a_val & b_val;
                    break;

                case 4: // 或 A | B
                    res = a_val | b_val;
                    break;

                case 5: // 异或 A ^ B
                    res = a_val ^ b_val;
                    break;

                case 6: // 比较大小 A < B ? 1 : 0
                    res = (a_val < b_val) ? 1 : 0;
                    break;

                case 7: // 判断相等 A == B ? 1 : 0
                    res = (a_val == b_val) ? 1 : 0;
                    break;

                case 8: // 乘法 A * B
                {
                    sc_int<16> prod = (sc_int<16>)a_val * (sc_int<16>)b_val;
                    res = prod.range(7, 0);
                    ov_flag = (prod > 127) || (prod < -128);
                }
                break;

                case 9: // 除法 A / B
                    if (b_val == 0) {
                        res = 0;
                        ov_flag = true;        // 除0视为溢出
                    } else {
                        res = a_val / b_val;   // 整数除法，向零截断
                    }
                    break;

                default:
                    res = 0;
                    break;
            }

            // 更新输出
            result.write(res);
            zero.write(res == 0);
            overflow.write(ov_flag);
            carry.write(cy_flag);
        }
    }

    // ==================== 构造函数 ====================
    SC_CTOR(alu_8bit_exam) {
        SC_CTHREAD(alu_process, clk.pos());
        reset_signal_is(rst_n, false);   // 声明低电平复位
    }
};

#endif // ALU_8BIT_EXAM_H