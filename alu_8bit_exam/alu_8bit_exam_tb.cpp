// alu_8bit_exam_tb.cpp
// 8位带符号补码 ALU 测试平台（优化版）
// Copyright (C) 2025  ZhaoCake

#include <systemc.h>
#include <iomanip>
#include <bitset>
#include <iostream>
#include "alu_8bit_exam.h"

SC_MODULE(alu_8bit_exam_tb) {
    // ==================== 信号声明 ====================
    sc_clock clk;
    sc_signal<bool> rst_n;
    sc_signal<sc_int<8>> A_sig;
    sc_signal<sc_int<8>> B_sig;
    sc_signal<sc_uint<32>> op_sig;        // 改为4位，更合理
    sc_signal<sc_int<8>> result_sig;
    sc_signal<bool> zero_sig;
    sc_signal<bool> overflow_sig;
    sc_signal<bool> carry_sig;
    
    // 波形追踪文件
    sc_trace_file *tf;
    
    // ALU 实例
    alu_8bit_exam alu_inst;

    // ==================== 显示结果函数 ====================
    void display_result(sc_int<8> a, sc_int<8> b, sc_uint<4> op, 
                       sc_int<8> result, bool zero, bool overflow, bool carry) {
        
        const char* op_name[] = {
            "加法 (A+B)", 
            "减法 (A-B)", 
            "取反 (~A)", 
            "与 (A&B)", 
            "或 (A|B)", 
            "异或 (A^B)", 
            "比较大小 (A<B?1:0)", 
            "判断相等 (A==B?1:0)",
            "乘法 (A*B)", 
            "除法 (A/B)"
        };
        
        const char* op_str = (op.to_uint() < 10) ? op_name[op.to_uint()] : "未知操作";

        cout << sc_time_stamp() 
             << "  A = " << std::setw(4) << a.to_int() 
             << " (" << std::bitset<8>(a.to_uint()) << ") "
             << "B = " << std::setw(4) << b.to_int()
             << " (" << std::bitset<8>(b.to_uint()) << ") "
             << "Op = " << op.to_string() 
             << " [" << op_str << "]\n"
             << "结果: " << std::setw(4) << result.to_int()
             << " (" << std::bitset<8>(result.to_uint()) << ") "
             << "Zero: " << (zero ? "是" : "否")
             << "  Overflow: " << (overflow ? "是" : "否")
             << "  Carry: " << (carry ? "是" : "否")
             << "\n-----------------------------------------\n";
    }

    // ==================== 测试进程 ====================
    void test_process() {
        cout << "\n====== 8位带符号补码 ALU 测试开始 ======\n\n";

        // ==================== 测试用例（优化后） ====================
        struct {
            int a, b;
            int op;
        } test_cases[] = {
            // 加法
            {3, 4, 0},
            {-3, -4, 0},
            {100, 50, 0},      // 正溢出
            {-100, -50, 0},    // 负溢出

            // 减法
            {5, 3, 1},
            {3, 5, 1},
            {-3, -5, 1},
            {-5, -3, 1},
            {100, -50, 1},     // 正溢出
            {-100, 50, 1},     // 负溢出

            // 取反
            {5, 0, 2},
            {-5, 0, 2},

            // 逻辑运算
            {0b00001111, 0b00110011, 3},   // 与
            {0b00001111, 0b00110011, 4},   // 或
            {0b00001111, 0b00110011, 5},   // 异或

            // 比较和判等
            {3, 5, 6},
            {5, 3, 6},
            {-3, 5, 6},
            {3, -5, 6},
            {10, 10, 7},
            {10, 20, 7},

            // 乘法（增加溢出测试）
            {3, 4, 8},
            {-3, -4, 8},
            {20, 10, 8},       // 200 → 溢出
            {-20, 10, 8},      // -200 → 溢出

            // 除法
            {6, 3, 9},
            {7, 2, 9},         // 整数除法 7/2 = 3
            {-6, 3, 9},
            {6, -2, 9},
            {1, 0, 9}          // 除0测试
        };

        // 复位
        rst_n.write(0);
        for (int i = 0; i < 5; i++) wait(clk.posedge_event());

        rst_n.write(1);
        wait(clk.posedge_event());

        // 执行所有测试用例
        for (auto &tc : test_cases) {
            A_sig.write(tc.a);
            B_sig.write(tc.b);
            op_sig.write(tc.op);

            wait(clk.posedge_event());     // 等待ALU在时钟沿更新输出
            wait(clk.posedge_event());     // 等待ALU在时钟沿更新输出

            wait(1, SC_NS);
            display_result(A_sig.read(), B_sig.read(), op_sig.read(),
                           result_sig.read(), zero_sig.read(),
                           overflow_sig.read(), carry_sig.read());
        }
        
        cout << "\n====== ALU 测试完成！======\n";
        cout << "波形文件已保存到 alu_8bit.vcd\n";
        sc_stop();
    }

    // ==================== 构造函数 ====================
    SC_CTOR(alu_8bit_exam_tb)
    : clk("clk", 10, SC_NS),
      alu_inst("alu_instance")
    {
        // 端口绑定
        alu_inst.clk(clk);
        alu_inst.rst_n(rst_n);
        alu_inst.A(A_sig);
        alu_inst.B(B_sig);
        alu_inst.op(op_sig);
        alu_inst.result(result_sig);
        alu_inst.zero(zero_sig);
        alu_inst.overflow(overflow_sig);
        alu_inst.carry(carry_sig);

        // 注册测试进程
        SC_THREAD(test_process);

        // 波形记录
        tf = sc_create_vcd_trace_file("alu_8bit_exam");
        tf->set_time_unit(1, SC_NS);

        sc_trace(tf, clk, "clk");
        sc_trace(tf, rst_n, "rst_n");
        sc_trace(tf, A_sig, "A");
        sc_trace(tf, B_sig, "B");
        sc_trace(tf, op_sig, "Op");
        sc_trace(tf, result_sig, "Result");
        sc_trace(tf, zero_sig, "Zero");
        sc_trace(tf, overflow_sig, "Overflow");
        sc_trace(tf, carry_sig, "Carry");
    }

    ~alu_8bit_exam_tb() {
        sc_close_vcd_trace_file(tf);
    }
};

int sc_main(int argc, char* argv[]) {
    alu_8bit_exam_tb tb("alu_testbench");
    sc_start();
    return 0;
}