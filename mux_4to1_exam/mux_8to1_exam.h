// File: mux_4to1.h
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

#ifndef MUX_8TO1_EXAM_H
#define MUX_8TO1_EXAM_H

#include <systemc.h>

SC_MODULE(mux_8to1_exam) {
    // 输入端口
    sc_in<sc_uint<4>> X0;    // 4位输入0
    sc_in<sc_uint<4>> X1;    // 4位输入1
    sc_in<sc_uint<4>> X2;    // 4位输入2
    sc_in<sc_uint<4>> X3;    // 4位输入3
    sc_in<sc_uint<4>> X4;    // 4位输入4
    sc_in<sc_uint<4>> X5;    // 4位输入5
    sc_in<sc_uint<4>> X6;    // 4位输入6
    sc_in<sc_uint<4>> X7;    // 4位输入7
    sc_in<bool> EN;          // 使能信号
    sc_in<sc_uint<3>> Y; // 8位选择输入

    // 输出端口
    sc_out<sc_uint<4>> F;    // 4位输出
    
    void mux_process() {
        // switch (Y.read()) {
        //     case 0: F.write(X0.read()); break;  // Y=00, 选择X0
        //     case 1: F.write(X1.read()); break;  // Y=01, 选择X1
        //     case 2: F.write(X2.read()); break;  // Y=10, 选择X2
        //     case 3: F.write(X3.read()); break;  // Y=11, 选择X3
        //     default: F.write(0); break;         // 默认情况（不应发生）
        if(EN.read()) {
            if(Y.read()==0)
                F.write(X0.read());
            else if(Y.read()==1)
                F.write(X1.read());
            else if(Y.read()==2)
                F.write(X2.read());
            else if(Y.read()==3)
                F.write(X3.read());
            else if(Y.read()==4)
                F.write(X4.read());
            else if(Y.read()==5)
                F.write(X5.read());
            else if(Y.read()==6)
                F.write(X6.read());
            else if(Y.read()==7)
                F.write(X7.read());
            else
                F.write(0);
        } else {
            F.write(0); // 当使能信号无效时，输出为0
        }
    }
    
    // 构造函数
    SC_CTOR(mux_8to1_exam) {
        // 注册进程
        SC_METHOD(mux_process);
        sensitive << X0 << X1 << X2 << X3 << X4 << X5 << X6 << X7 << Y << EN;  // 对所有输入敏感
    }
};

#endif // MUX_8TO1_EXAM_H
