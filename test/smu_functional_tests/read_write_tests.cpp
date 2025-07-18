#include <iostream>
#include "include/SecureMemoryUnit_debug.hpp"

int main() {
    // Set up the request manually
    Request reqs[20];
    uint32_t test_data[10] = {
            0x00000000,    // Minimum
            0xFFFFFFFF,    // Maximum
            0x00000001,    // Single bit set low
            0x80000000,    // Single bit set high
            0xAAAAAAAA,    // Alternating bits 1010...
            0x55555555,    // Alternating bits 0101...
            0xFF000000,    // Only high byte
            0x000000FF,    // Only low byte
            0xCAFEBABE,    // Equals to key (example)
            0xA0           // Equals to address
    };
    uint32_t test_addr[10] = {
            0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80, 0x90, 0xA0
    };


    for (int i = 0; i < 10; ++i) {
        // Write request
        reqs[2*i].addr = test_addr[i];
        reqs[2*i].data = test_data[i];
        reqs[2*i].r = 0;
        reqs[2*i].w = 1;
        reqs[2*i].fault = UINT32_MAX;
        reqs[2*i].faultBit = 0;

        // Read request
        reqs[2*i+1].addr = test_addr[i];
        reqs[2*i+1].data = 0;
        reqs[2*i+1].r = 1;
        reqs[2*i+1].w = 0;
        reqs[2*i+1].fault = UINT32_MAX;
        reqs[2*i+1].faultBit = 0;
    }

    // Use the key as seed if you want to match the "equals to key" case
    uint32_t test_key = 0xCAFEBABE;

    Result result = run_simulation(
            10000,           // max_cycles
            "r&w_trace",  // tracefile
            0,               // endianness
            1,               // latency_scrambling
            1,               // latency_encryption
            1,               // latency_memory_access
            test_key,        // seed
            20,              // numRequests
            reqs
    );

    std::cout << "Testbench Simulation completed.\n";
    std::cout << "Cycles: " << result.cycles << std::endl;
    std::cout << "Errors: " << result.errors << std::endl;

    return 0;
}
