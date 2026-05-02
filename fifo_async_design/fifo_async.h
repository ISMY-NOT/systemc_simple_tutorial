#ifndef FIFO_ASYNC_H
#define FIFO_ASYNC_H

#include <systemc.h>

template<int DW = 8, int AW = 4>  // 数据位宽、地址位宽（深度 = 2^AW）
SC_MODULE(fifo_async) {

    // =====================
    // 端口
    // =====================
    sc_in<bool> clk_w, rst_w;
    sc_in<bool> clk_r, rst_r;

    sc_in<bool> wr_en;
    sc_in<sc_uint<DW>> data_in;
    sc_out<bool> full;

    sc_in<bool> rd_en;
    sc_out<sc_uint<DW>> data_out;
    sc_out<bool> empty;

    // =====================
    // 内部存储
    // =====================
    sc_uint<DW> mem[1 << AW];

    // =====================
    // 指针（binary + gray）
    // =====================
    sc_signal<sc_uint<AW+1>> w_ptr_bin, w_ptr_gray;
    sc_signal<sc_uint<AW+1>> r_ptr_bin, r_ptr_gray;

    // 同步后的指针
    sc_signal<sc_uint<AW+1>> w_ptr_gray_sync1, w_ptr_gray_sync2;
    sc_signal<sc_uint<AW+1>> r_ptr_gray_sync1, r_ptr_gray_sync2;

    // =====================
    // Gray 编码函数
    // =====================
    sc_uint<AW+1> bin2gray(sc_uint<AW+1> bin) {
        return (bin >> 1) ^ bin;
    }

    // =====================
    // 写时钟域逻辑
    // =====================
    void write_proc() {
        if (rst_w.read() == 0) {
            w_ptr_bin.write(0);
            w_ptr_gray.write(0);
        } else {
            if (wr_en.read() && !full.read()) {
                sc_uint<AW> addr = w_ptr_bin.read().range(AW-1, 0);
                mem[addr] = data_in.read();

                sc_uint<AW+1> next = w_ptr_bin.read() + 1;
                w_ptr_bin.write(next);
                w_ptr_gray.write(bin2gray(next));
            }
        }
    }

    // =====================
    // 读时钟域逻辑
    // =====================
    void read_proc() {
        if (rst_r.read() == 0) {
            r_ptr_bin.write(0);
            r_ptr_gray.write(0);
            data_out.write(0);
        } else {
            if (rd_en.read() && !empty.read()) {
                sc_uint<AW> addr = r_ptr_bin.read().range(AW-1, 0);
                data_out.write(mem[addr]);

                sc_uint<AW+1> next = r_ptr_bin.read() + 1;
                r_ptr_bin.write(next);
                r_ptr_gray.write(bin2gray(next));
            }
        }
    }

    // =====================
    // 指针同步（跨时钟）
    // =====================

    // r_ptr 同步到 clk_w 域
    void sync_r2w() {
        r_ptr_gray_sync1.write(r_ptr_gray.read());
        r_ptr_gray_sync2.write(r_ptr_gray_sync1.read());
    }

    // w_ptr 同步到 clk_r 域
    void sync_w2r() {
        w_ptr_gray_sync1.write(w_ptr_gray.read());
        w_ptr_gray_sync2.write(w_ptr_gray_sync1.read());
    }

    // =====================
    // Full 判断（写域）
    // =====================
    void full_proc() {
        sc_uint<AW+1> w = w_ptr_gray.read();
        sc_uint<AW+1> r = r_ptr_gray_sync2.read();

        // 判断满（经典：最高两位取反）
        sc_uint<AW+1> r_inv = r;
        r_inv[AW] = ~r[AW];
        r_inv[AW-1] = ~r[AW-1];

        full.write(w == r_inv);
    }

    // =====================
    // Empty 判断（读域）
    // =====================
    void empty_proc() {
        empty.write(r_ptr_gray.read() == w_ptr_gray_sync2.read());
    }

    // =====================
    // 构造函数
    // =====================
    SC_CTOR(async_fifo) {

        SC_METHOD(write_proc);
        sensitive << clk_w.pos();

        SC_METHOD(read_proc);
        sensitive << clk_r.pos();

        SC_METHOD(sync_r2w);
        sensitive << clk_w.pos();

        SC_METHOD(sync_w2r);
        sensitive << clk_r.pos();

        SC_METHOD(full_proc);
        sensitive << w_ptr_gray << r_ptr_gray_sync2;

        SC_METHOD(empty_proc);
        sensitive << r_ptr_gray << w_ptr_gray_sync2;
    }
};

#endif // FIFO_ASYNC_H
