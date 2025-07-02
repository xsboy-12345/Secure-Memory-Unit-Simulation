#ifndef SECURE_MEMORY_UNIT_SIMULATION_PARITYCHECKER_HPP
#define SECURE_MEMORY_UNIT_SIMULATION_PARITYCHECKER_HPP
#include <systemc.h>

SC_MODULE(ParityChecker) {
        sc_in<sc_uint<32>> data_in;
        sc_out<bool> parity_out;

        void compute_parity() {
            parity_out.write(data_in.read().xor_reduce());
        }

        SC_CTOR(ParityChecker) {
            SC_METHOD(compute_parity);
            sensitive << data_in;
        }
};
#endif //SECURE_MEMORY_UNIT_SIMULATION_PARITYCHECKER_HPP
