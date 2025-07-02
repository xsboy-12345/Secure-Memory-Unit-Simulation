#ifndef SECURE_MEMORY_UNIT_SIMULATION_ADDRESSSCRAMBLER_HPP
#define SECURE_MEMORY_UNIT_SIMULATION_ADDRESSSCRAMBLER_HPP
#include <systemc.h>

SC_MODULE(AddressScrambler) {
        sc_in<sc_uint<32>> addr_in;
        sc_in<sc_uint<32>> key_in;
        sc_out<sc_uint<32>> addr_out;

        void scramble_process() {
            addr_out.write(addr_in.read() ^ key_in.read());  // 按位 XOR 加扰
        }

        SC_CTOR(AddressScrambler) {
            SC_METHOD(scramble_process);
            sensitive << addr_in << key_in;
        }
};
#endif //SECURE_MEMORY_UNIT_SIMULATION_ADDRESSSCRAMBLER_HPP
