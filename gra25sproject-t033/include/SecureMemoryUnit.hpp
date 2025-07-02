#ifndef SECURE_MEMORY_UNIT_SIMULATION_SECURE_MEMORY_UNIT_HPP
#define SECURE_MEMORY_UNIT_SIMULATION_SECURE_MEMORY_UNIT_HPP
#include <systemc.h>
#include "Encryptor.hpp"
#include "AddressScrambler.hpp"
#include "ParityChecker .hpp"

SC_MODULE(SecureMemoryUnit) {
    sc_in<bool> clk;
    sc_in<bool> r, w;
    sc_in<uint32_t> addr, wdata;
    sc_in<uint32_t> fault;
    sc_in<sc_bv<5>> faultBit;

    sc_out<uint32_t> rdata;
    sc_out<bool> ready, error;

    static const int MEM_SIZE = 1024;
    sc_uint<32> memory[MEM_SIZE];
    bool parity_store[MEM_SIZE];//一个内存条对应奇偶校验池中的一位

    sc_signal<uint32_t> scramble_key;
    sc_signal<uint32_t> encryption_key;

    //子模块指针，所有模块的子模块实例必须使用指针 (new) 来动态分配
    AddressScrambler* scrambler;
    Encryptor* encryptor_w;
    Encryptor* encryptor_r;
    ParityChecker* parity_gen;
    ParityChecker* parity_chk;

    sc_signal<uint32_t> scrambled_addr;
    sc_signal<uint32_t> encrypted_data;
    sc_signal<uint32_t> decrypted_data;
    sc_signal<bool> parity_bit_gen;
    sc_signal<bool> parity_bit_chk;

    void control_process();
    SC_CTOR(SecureMemoryUnit);
}
#endif //SECURE_MEMORY_UNIT_SIMULATION_SECURE_MEMORY_UNIT_HPP
