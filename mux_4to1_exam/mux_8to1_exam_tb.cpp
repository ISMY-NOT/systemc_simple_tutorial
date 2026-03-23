// File: mux_4to1_tb.cpp
// Copyright (C) 2025  ZhaoCake

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include <systemc.h>
#include "mux_8to1_exam.h"
#include "random"

SC_MODULE(mux_8to1_exam_tb) {
    // 信号
    sc_signal<sc_uint<4>> X0_sig;
    sc_signal<sc_uint<4>> X1_sig;
    sc_signal<sc_uint<4>> X2_sig;
    sc_signal<sc_uint<4>> X3_sig;
    sc_signal<sc_uint<4>> X4_sig;
    sc_signal<sc_uint<4>> X5_sig;
    sc_signal<sc_uint<4>> X6_sig;
    sc_signal<sc_uint<4>> X7_sig;
    sc_signal<bool> EN_sig;
    sc_signal<sc_uint<3>> Y_sig;
    sc_signal<sc_uint<4>> F_sig;
    
    // 波形追踪文件指针
    sc_trace_file *tf;

    // 组件实例
    mux_8to1_exam mux_inst;
    
    // 测试进程
    void test_process() {
        // 初始化输入，每个输入赋予不同值便于观察
        X0_sig.write(8);   // 1000
        X1_sig.write(9);   // 1001
        X2_sig.write(10);  // 1010
        X3_sig.write(11);  // 1011
        X4_sig.write(12);  // 1100
        X5_sig.write(13);  // 1101
        X6_sig.write(14);  // 1110
        X7_sig.write(15);  // 1111
        
        cout << "==== 4位8选1选择器功能测试 ====" << endl;
        cout << "时间\tY\tF" << endl;
        cout << "------------------------" << endl;
        
        // 测试所有选择组合
        for (int i = 0; i < 8; i++) {
            Y_sig.write(i);
            EN_sig.write(1);
            wait(10, SC_NS); // 等待信号传播

            cout << sc_time_stamp() << "\t" 
                 << Y_sig.read() << "\t" 
                 << F_sig.read() << endl;
            
            // 验证输出是否正确
            sc_assert(F_sig.read() == i+8);
        }


        cout << "==== 4位8选1选择器使能测试 ====" << endl;
        cout << "时间\tY\tF" << endl;
        cout << "------------------------" << endl;

        // 测试使能信号
        for (int i = 0; i < 8; i++) {
            Y_sig.write(i);

            std::random_device rd;              // 随机种子
            std::mt19937 gen(rd());             // 随机数引擎
            std::uniform_int_distribution<> dis(0, 1);  // 生成 0 或 1
            bool enable = dis(gen);  // 0 -> false, 1 -> true

            EN_sig.write(enable);

            wait(10, SC_NS);  // 等待信号传播

            if(EN_sig){
            cout << sc_time_stamp() << "\t" 
                << Y_sig.read() << "\t" 
                << F_sig.read() << endl;
            
            // 验证输出是否正确
            sc_assert(F_sig.read() == i+8);}
            else {
                cout << sc_time_stamp() << "\t"
                << Y_sig.read() << "\t"
                << "未使能" << endl;
            }
            
        }
        
        cout << "测试成功完成!" << endl;
        cout << "波形已保存到 mux_4to1.vcd 文件" << endl;
        sc_stop();
    }
    
    // 构造函数
    SC_CTOR(mux_8to1_exam_tb)
    : mux_inst("mux_instance") {
        // 连接信号到被测设备
        mux_inst.X0(X0_sig);
        mux_inst.X1(X1_sig);
        mux_inst.X2(X2_sig);
        mux_inst.X3(X3_sig);
        mux_inst.X4(X4_sig);
        mux_inst.X5(X5_sig);
        mux_inst.X6(X6_sig);
        mux_inst.X7(X7_sig);
        mux_inst.EN(EN_sig);
        mux_inst.Y(Y_sig);
        mux_inst.F(F_sig);
        
        // 注册进程
        SC_THREAD(test_process);
        
        // 创建波形追踪文件
        tf = sc_create_vcd_trace_file("mux_8to1_exam");
        
        // 设置波形时间单位
        tf->set_time_unit(1, SC_NS);
        
        // 添加信号到波形文件
        sc_trace(tf, X0_sig, "X0");
        sc_trace(tf, X1_sig, "X1");
        sc_trace(tf, X2_sig, "X2");
        sc_trace(tf, X3_sig, "X3");
        sc_trace(tf, X4_sig, "X4");
        sc_trace(tf, X5_sig, "X5");
        sc_trace(tf, X6_sig, "X6");
        sc_trace(tf, X7_sig, "X7");
        sc_trace(tf, EN_sig, "EN");
        sc_trace(tf, Y_sig, "Y");
        sc_trace(tf, F_sig, "F");
    }
    
    // 析构函数
    ~mux_8to1_exam_tb() {
        // 关闭波形文件
        sc_close_vcd_trace_file(tf);
    }
};

int sc_main(int argc, char* argv[]) {
    mux_8to1_exam_tb tb("testbench");
    sc_start();
    return 0;
}
